#!/usr/bin/env python3
"""
serial_mux_gui.py - Cross-platform GUI for the serial channel multiplexer.

Runs on Linux, Windows and macOS (tkinter ships with CPython). Instead of
creating OS-level virtual serial ports (which Windows cannot do natively),
the GUI *is* the N virtual terminals: each channel gets its own tab with a
receive log and a transmit box. All channels share one real UART, using the
same COBS channel-framing protocol as serial_mux_hub.py, so the far end
(another PC running the hub, or MCU firmware) can tell the 4 sources apart.

    channel 0..N-1  <-- tab in this GUI
                     |
                     v   COBS( channel_byte + payload ) 0x00
                real UART (e.g. COM3 / /dev/ttyUSB0)

Requires: pyserial  (pip install pyserial)
Run:      python3 serial_mux_gui.py
"""

import os
import queue
import sys
import threading

import tkinter as tk
from tkinter import messagebox, ttk

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from serial_mux_hub import build_frame, cobs_decode  # shared framing core


def list_serial_ports():
    """Return a list of (device, description) tuples, empty if pyserial missing."""
    try:
        from serial.tools import list_ports
    except Exception:
        return []
    return [(p.device, p.description or p.device) for p in list_ports.comports()]


class SerialMuxGui:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial MUX - N channels over 1 UART")

        self.serial = None
        self.reader_thread = None
        self.stop_flag = threading.Event()
        self.rx_queue = queue.Queue()      # (channel, bytes) from reader thread
        self.status_queue = queue.Queue()  # status strings from reader thread
        self._rx_buf = bytearray()
        self.channel_views = {}            # channel -> scrolledtext widget
        self.max_payload = 240

        self._build_config_bar()
        self._build_channel_tabs(4)

        self.root.protocol("WM_DELETE_WINDOW", self._on_close)
        self.root.after(50, self._pump_queues)

    # ---------------------------------------------------------------- UI
    def _build_config_bar(self):
        bar = ttk.Frame(self.root, padding=6)
        bar.pack(fill="x")

        ttk.Label(bar, text="Port:").pack(side="left")
        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(bar, textvariable=self.port_var, width=22,
                                       values=[], state="readonly")
        self.port_combo.pack(side="left", padx=(2, 6))

        ttk.Button(bar, text="Refresh", command=self._refresh_ports).pack(side="left")

        ttk.Label(bar, text="Baud:").pack(side="left", padx=(10, 0))
        self.baud_var = tk.StringVar(value="115200")
        ttk.Combobox(bar, textvariable=self.baud_var, width=9,
                     values=["9600", "19200", "38400", "57600", "115200",
                             "230400", "460800", "921600"]).pack(side="left", padx=2)

        ttk.Label(bar, text="Channels:").pack(side="left", padx=(10, 0))
        self.chan_var = tk.StringVar(value="4")
        chan_box = ttk.Combobox(bar, textvariable=self.chan_var, width=4,
                                values=[str(n) for n in range(1, 17)],
                                state="readonly")
        chan_box.pack(side="left", padx=2)
        chan_box.bind("<<ComboboxSelected>>", self._on_channels_changed)

        self.connect_btn = ttk.Button(bar, text="Connect", command=self._toggle_connect)
        self.connect_btn.pack(side="left", padx=(12, 0))

        self.status_var = tk.StringVar(value="disconnected")
        ttk.Label(bar, textvariable=self.status_var, foreground="#888").pack(
            side="right")

        self._refresh_ports()

    def _build_channel_tabs(self, channels):
        if hasattr(self, "notebook"):
            self.notebook.destroy()
        self.channel_views.clear()

        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill="both", expand=True, padx=6, pady=(0, 6))

        for ch in range(channels):
            tab = ttk.Frame(self.notebook)
            self.notebook.add(tab, text=f"Channel {ch}")

            log = tk.Text(tab, height=18, wrap="none", state="disabled",
                          background="#111", foreground="#0f0",
                          insertbackground="#0f0")
            yscroll = ttk.Scrollbar(tab, orient="vertical", command=log.yview)
            log.configure(yscrollcommand=yscroll.set)
            log.grid(row=0, column=0, sticky="nsew")
            yscroll.grid(row=0, column=1, sticky="ns")

            entry_row = ttk.Frame(tab)
            entry_row.grid(row=1, column=0, columnspan=2, sticky="ew", pady=4)
            entry = ttk.Entry(entry_row)
            entry.pack(side="left", fill="x", expand=True, padx=(0, 4))
            nl_var = tk.BooleanVar(value=True)
            ttk.Checkbutton(entry_row, text="\\n", variable=nl_var).pack(side="left")

            def make_send(c=ch, e=entry, nlv=nl_var):
                return lambda *_: self._send_channel(c, e, nlv)

            entry.bind("<Return>", make_send())
            ttk.Button(entry_row, text="Send", command=make_send()).pack(
                side="left", padx=4)

            tab.rowconfigure(0, weight=1)
            tab.columnconfigure(0, weight=1)
            self.channel_views[ch] = log

    # ---------------------------------------------------------------- actions
    def _refresh_ports(self):
        ports = list_serial_ports()
        values = [f"{dev}  ({desc})" for dev, desc in ports]
        self._port_map = {f"{dev}  ({desc})": dev for dev, desc in ports}
        self.port_combo["values"] = values
        if values and not self.port_var.get():
            self.port_var.set(values[0])

    def _on_channels_changed(self, *_):
        if self.serial is not None:
            messagebox.showinfo("Serial MUX",
                                "Disconnect before changing channel count.")
            return
        self._build_channel_tabs(int(self.chan_var.get()))

    def _toggle_connect(self):
        if self.serial is None:
            self._connect()
        else:
            self._disconnect()

    def _connect(self):
        try:
            import serial  # pyserial
        except Exception:
            messagebox.showerror("Serial MUX",
                                 "pyserial not installed.\n\npip install pyserial")
            return

        label = self.port_var.get()
        dev = getattr(self, "_port_map", {}).get(label, label)
        if not dev:
            messagebox.showerror("Serial MUX", "Select a serial port first.")
            return
        try:
            self.serial = serial.Serial(dev, int(self.baud_var.get()), timeout=0.1)
        except Exception as exc:
            messagebox.showerror("Serial MUX", f"Open failed:\n{exc}")
            self.serial = None
            return

        self.channels = int(self.chan_var.get())
        self._rx_buf = bytearray()
        self.stop_flag.clear()
        self.reader_thread = threading.Thread(target=self._reader_loop, daemon=True)
        self.reader_thread.start()

        self.connect_btn.config(text="Disconnect")
        self.status_var.set(f"connected {dev} @ {self.baud_var.get()}")
        self.port_combo.config(state="disabled")

    def _disconnect(self):
        self.stop_flag.set()
        if self.reader_thread:
            self.reader_thread.join(timeout=1.0)
            self.reader_thread = None
        if self.serial:
            try:
                self.serial.close()
            except Exception:
                pass
            self.serial = None
        self.connect_btn.config(text="Connect")
        self.status_var.set("disconnected")
        self.port_combo.config(state="readonly")

    def _send_channel(self, channel, entry, nl_var):
        if self.serial is None:
            messagebox.showinfo("Serial MUX", "Not connected.")
            return
        text = entry.get()
        if nl_var.get():
            text += "\n"
        payload = text.encode("utf-8", "replace")
        if not payload:
            return
        try:
            for i in range(0, len(payload), self.max_payload):
                chunk = payload[i:i + self.max_payload]
                self.serial.write(build_frame(channel, chunk))
        except Exception as exc:
            self.status_queue.put(f"write error: {exc}")
            return
        self._append(channel, f">> {text}" if nl_var.get() else f">> {text}\n",
                     tx=True)
        entry.delete(0, "end")

    # ---------------------------------------------------------------- reader
    def _reader_loop(self):
        while not self.stop_flag.is_set():
            try:
                data = self.serial.read(4096)
            except Exception as exc:
                self.status_queue.put(f"read error: {exc}")
                break
            if not data:
                continue
            self._rx_buf.extend(data)
            while True:
                try:
                    zero = self._rx_buf.index(0)
                except ValueError:
                    break
                body = bytes(self._rx_buf[:zero])
                del self._rx_buf[:zero + 1]
                if not body:
                    continue
                try:
                    decoded = cobs_decode(body)
                except ValueError:
                    continue
                if not decoded:
                    continue
                ch = decoded[0]
                payload = decoded[1:]
                if 0 <= ch < self.channels and payload:
                    self.rx_queue.put((ch, payload))

    # ---------------------------------------------------------------- pumps
    def _pump_queues(self):
        try:
            while True:
                ch, payload = self.rx_queue.get_nowait()
                text = payload.decode("utf-8", "replace")
                self._append(ch, text, tx=False)
        except queue.Empty:
            pass
        try:
            while True:
                self.status_var.set(self.status_queue.get_nowait())
        except queue.Empty:
            pass
        self.root.after(50, self._pump_queues)

    def _append(self, channel, text, tx=False):
        log = self.channel_views.get(channel)
        if log is None:
            return
        log.config(state="normal")
        log.insert("end", text)
        log.see("end")
        log.config(state="disabled")

    def _on_close(self):
        self._disconnect()
        self.root.destroy()


def main():
    root = tk.Tk()
    root.geometry("720x480")
    SerialMuxGui(root)
    root.mainloop()


if __name__ == "__main__":
    main()

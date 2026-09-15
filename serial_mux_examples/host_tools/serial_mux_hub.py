#!/usr/bin/env python3
"""
serial_mux_hub.py - Multiplex several virtual serial ports over one real UART.

Creates N virtual serial ports (Linux PTYs, e.g. /tmp/vcom0../tmp/vcom3).
Everything an application writes into a virtual port is framed with a
1-byte channel id and sent over one physical port (e.g. /dev/ttyUSB0).
Frames received from the physical port are de-framed and routed back to
the matching virtual port, so each application only ever sees its own
channel's byte stream.

Wire framing (per frame):

    [ COBS( <channel_byte> + <payload...> ) ] 0x00

  * COBS (Consistent Overhead Byte Stuffing) removes all 0x00 bytes from
    the encoded body, so a single 0x00 is a reliable, self-synchronizing
    frame delimiter. A corrupted / half frame is discarded and the
    receiver re-syncs on the next 0x00 with no ambiguity.
  * The first decoded byte is the channel id (0..N-1); the rest is payload.

The SAME framing is what the far end must implement (another PC running
this script, or firmware). A minimal C decoder is described in the README.

Usage:
    python3 serial_mux_hub.py --port /dev/ttyUSB0 --baud 115200 --channels 4

Then point 4 applications at:
    /tmp/vcom0  -> channel 0
    /tmp/vcom1  -> channel 1
    /tmp/vcom2  -> channel 2
    /tmp/vcom3  -> channel 3

Requires: pyserial  (pip install pyserial)
Linux/macOS only (uses os.openpty()). For Windows use com0com + this logic.
"""

import argparse
import os
import sys

# NOTE: pty / termios / tty / select / serial are Linux-only or require
# pyserial. They are imported lazily inside the hub so this module's pure
# COBS framing (cobs_encode/decode, build_frame) can be imported and unit
# tested on any platform (e.g. Windows) without those dependencies.



# ----------------------------------------------------------------------------
# COBS encode / decode (no external dependency)
# ----------------------------------------------------------------------------
def cobs_encode(data: bytes) -> bytes:
    """Encode data so the output contains no 0x00 bytes."""
    out = bytearray()
    idx = 0
    n = len(data)
    while True:
        # find next zero (or end) within a 254-byte window
        block_end = idx
        code = 1
        while block_end < n and data[block_end] != 0 and code < 0xFF:
            block_end += 1
            code += 1
        out.append(code)
        out.extend(data[idx:block_end])
        if block_end < n and data[block_end] == 0:
            idx = block_end + 1
            if idx == n:
                # trailing zero -> need a final empty block
                out.append(1)
                break
        else:
            idx = block_end
            if idx >= n:
                break
    return bytes(out)


def cobs_decode(data: bytes) -> bytes:
    """Inverse of cobs_encode. Raises ValueError on malformed input."""
    out = bytearray()
    idx = 0
    n = len(data)
    while idx < n:
        code = data[idx]
        if code == 0:
            raise ValueError("unexpected zero in COBS body")
        idx += 1
        block_len = code - 1
        if idx + block_len > n:
            raise ValueError("COBS block overruns buffer")
        out.extend(data[idx:idx + block_len])
        idx += block_len
        if code < 0xFF and idx < n:
            out.append(0)
    return bytes(out)


def build_frame(channel: int, payload: bytes) -> bytes:
    return cobs_encode(bytes([channel]) + payload) + b"\x00"


# ----------------------------------------------------------------------------
# Hub
# ----------------------------------------------------------------------------
class SerialMuxHub:
    def __init__(self, port, baud, channels, link_prefix, max_payload,
                 debug=False):
        # Linux-only / pyserial deps, imported here so the module's pure COBS
        # framing stays importable on any platform.
        import pty
        import select
        import termios
        import tty
        import serial  # pyserial

        # publish to module globals used by _make_raw() and run()
        global termios, tty, select  # noqa: PLW0603
        self._pty = pty

        self.channels = channels
        self.max_payload = max_payload
        self.link_prefix = link_prefix
        self.debug = debug

        # Open the one real UART.
        self.uart = serial.Serial(port, baud, timeout=0)

        self.uart_fd = self.uart.fileno()

        # rx assembly buffer for physical -> virtual direction
        self._rx = bytearray()

        # Create N PTYs. master_fd is what the hub reads/writes; the slave
        # side is exposed as a stable symlink /tmp/vcomN for applications.
        self.master_fds = []
        self.links = []
        for ch in range(channels):
            master_fd, slave_fd = pty.openpty()
            self._make_raw(slave_fd)
            slave_name = os.ttyname(slave_fd)
            link = f"{link_prefix}{ch}"
            try:
                if os.path.islink(link) or os.path.exists(link):
                    os.remove(link)
            except OSError:
                pass
            os.symlink(slave_name, link)
            os.close(slave_fd)  # keep only the symlink open on demand
            self.master_fds.append(master_fd)
            self.links.append(link)
            print(f"channel {ch}: {link} -> {slave_name}")

    def _dbg(self, msg):
        print(msg, file=sys.stderr, flush=True)

    @staticmethod
    def _make_raw(fd):
        try:
            attrs = termios.tcgetattr(fd)
            tty.setraw(fd)
            termios.tcsetattr(fd, termios.TCSANOW, attrs and termios.tcgetattr(fd))
        except termios.error:
            pass

    def _fd_to_channel(self, fd):
        return self.master_fds.index(fd)

    def run(self):
        print(f"\nHub running. Physical port framed onto {self.uart.port} "
              f"@ {self.uart.baudrate} baud. Ctrl-C to stop.\n")
        if self.debug:
            self._dbg(f"[debug] RX instrumentation ON, channels=0..{self.channels - 1}")
        # Only watch the vport master fds via select(); the UART is drained
        # unconditionally every loop pass below. Some USB-UART adapters buffer
        # received bytes in their RX FIFO / latency timer and do NOT mark the
        # UART fd readable to select() until later bus activity flushes them,
        # which would otherwise stall the physical->virtual path (e.g. a boot
        # banner not appearing until the user types a key). The select timeout
        # wakes the loop even when no vport fd is readable, guaranteeing the
        # adapter backlog is read within one loop iteration.
        watch = self.master_fds
        try:
            while True:
                rlist, _, _ = select.select(watch, [], [], 0.1)
                # Always drain the UART, regardless of select() readiness.
                self._on_uart_data()
                for fd in rlist:
                    self._on_vcom_data(fd)
        except KeyboardInterrupt:
            print("\nStopping.")
        finally:
            self._cleanup()

    # virtual port -> physical (frame it)
    def _on_vcom_data(self, fd):
        ch = self._fd_to_channel(fd)
        try:
            data = os.read(fd, 4096)
        except OSError:
            return
        if not data:
            return
        # chunk to keep per-frame payload bounded
        for i in range(0, len(data), self.max_payload):
            chunk = data[i:i + self.max_payload]
            self.uart.write(build_frame(ch, chunk))

    # physical -> virtual (de-frame it)
    def _on_uart_data(self):
        data = self.uart.read(4096)
        if not data:
            return
        if self.debug:
            self._dbg(f"[rx] raw {len(data)} bytes: {data.hex()}")
        self._rx.extend(data)
        while True:
            try:
                zero = self._rx.index(0)
            except ValueError:
                if self.debug and self._rx:
                    self._dbg(f"[rx]   partial frame buffered "
                              f"({len(self._rx)} bytes, no 0x00 yet)")
                break  # no complete frame yet
            body = bytes(self._rx[:zero])
            del self._rx[:zero + 1]
            if self.debug:
                self._dbg(f"[rx]   frame body {len(body)} bytes: {body.hex()}")
            if not body:
                if self.debug:
                    self._dbg("[rx]   DROPPED: empty frame body (stray 0x00)")
                continue
            try:
                decoded = cobs_decode(body)
            except ValueError as exc:
                if self.debug:
                    self._dbg(f"[rx]   DROPPED: COBS decode failed: {exc}")
                continue  # corrupt frame, resync on next 0x00
            if not decoded:
                if self.debug:
                    self._dbg("[rx]   DROPPED: empty decode")
                continue
            ch = decoded[0]
            payload = decoded[1:]
            if self.debug:
                self._dbg(f"[rx]   decoded ch={ch} payload={len(payload)} "
                          f"bytes: {payload!r}")
            if 0 <= ch < self.channels and payload:
                try:
                    os.write(self.master_fds[ch], payload)
                    if self.debug:
                        self._dbg(f"[rx]   -> delivered to channel {ch} "
                                  f"({self.links[ch]})")
                except OSError as exc:
                    if self.debug:
                        self._dbg(f"[rx]   DROPPED: os.write to ch {ch} "
                                  f"failed: {exc}")
            elif self.debug:
                if not (0 <= ch < self.channels):
                    self._dbg(f"[rx]   DROPPED: channel {ch} out of range "
                              f"[0, {self.channels})")
                else:
                    self._dbg("[rx]   DROPPED: empty payload")

    def _cleanup(self):
        for fd in self.master_fds:
            try:
                os.close(fd)
            except OSError:
                pass
        for link in self.links:
            try:
                os.remove(link)
            except OSError:
                pass
        try:
            self.uart.close()
        except Exception:
            pass


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", default="/dev/ttyUSB0", help="real serial device")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--channels", type=int, default=4)
    ap.add_argument("--link-prefix", default="/tmp/vcom",
                    help="virtual port symlink prefix (creates <prefix>0..N-1)")
    ap.add_argument("--max-payload", type=int, default=240,
                    help="max payload bytes per frame")
    ap.add_argument("--debug", action="store_true",
                    help="print receive-direction diagnostics to stderr "
                         "(raw bytes, each frame body, COBS decode result, "
                         "decoded channel/length, and drop reason)")
    args = ap.parse_args()

    if args.channels < 1 or args.channels > 250:
        ap.error("--channels must be 1..250")

    hub = SerialMuxHub(args.port, args.baud, args.channels,
                       args.link_prefix, args.max_payload, args.debug)
    hub.run()


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
"""
serial_mux_vports.py - Expose N REAL, OS-level virtual serial ports that any
third-party software (minicom, PuTTY, TeraTerm, pyserial, LabVIEW, ...) can
open directly, all multiplexed over ONE physical UART with the shared COBS
channel-framing protocol from serial_mux_hub.py.

Naming (so a tool just opens the port by name):

  Linux / macOS   real port + "CHn" suffix as a PTY symlink, e.g.
                    /dev/ttyUSB0CH0 .. /dev/ttyUSB0CH3   (default)
                  or a fixed prefix:  /tmp/vcom0 .. /tmp/vcom3

  Windows         Windows CANNOT create a virtual COM port from user space.
                  Install com0com (https://com0com.sourceforge.net) and create
                  N port PAIRS. Give the APPLICATION side friendly names such
                  as COM3CH0, COM3CH1, ... and tell this bridge the OTHER
                  (bridge) side of each pair via --win-bridge-ports. This
                  bridge opens the bridge side; the app opens COM3CHn.

Wire framing (identical to serial_mux_hub.py):

    [ COBS( <channel_byte> + <payload...> ) ] 0x00

Examples
--------
Linux, auto names /dev/ttyUSB0CH0..3 :
    python3 serial_mux_vports.py --port /dev/ttyUSB0 --channels 4

Linux, explicit names :
    python3 serial_mux_vports.py --port /dev/ttyUSB0 \\
        --vport-names /tmp/ttyA,/tmp/ttyB,/tmp/ttyC,/tmp/ttyD

Windows, com0com pairs (app opens COM20/COM22, bridge opens COM21/COM23) :
    python serial_mux_vports.py --port COM3 --channels 2 \\
        --win-bridge-ports COM21,COM23

Windows, two hand-made pairs (e.g. COM1<->COM2 and COM3<->COM4) :
    Some virtual-serial tools (HHD Free Virtual Serial Ports, and any
    GUI-only tool) require you to create the pairs by hand. Each pair has
    two internally cross-connected ports; one side is opened by this
    bridge and the OTHER side is opened by the third-party application.
    Suppose you created two pairs:
        pair 1 : COM1 <-> COM2
        pair 2 : COM3 <-> COM4
    Pick one side of each pair as the bridge side (say COM2 and COM4);
    the app then opens COM1 and COM3. Pass the bridge-side ports, one
    per channel, in channel order :
        python serial_mux_vports.py --port COM14 --channels 2 \\
            --vsp-tool hhd --win-bridge-ports COM2,COM4
    This maps channel 0 -> bridge opens COM2 (app opens COM1) and
    channel 1 -> bridge opens COM4 (app opens COM3). The number of
    --win-bridge-ports entries must equal --channels.

Requires: pyserial  (pip install pyserial)
"""

import argparse
import os
import subprocess
import sys
import threading
import time
import urllib.request


sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from serial_mux_hub import build_frame, cobs_decode  # shared framing core


# When run as a background task, stdout/stderr may be discarded, so a
# silent startup exception (e.g. serial.Serial() failing to open the real
# port BEFORE any PTY/thread is created) looks exactly like "no [PC->MCU]
# log ever". Mirror every diagnostic to a logfile as well so the real cause
# is always recoverable.
_LOGFILE = os.environ.get("VPORTS_LOG", "/tmp/serial_mux_vports.log")


def _logline(msg):
    try:
        with open(_LOGFILE, "a") as fh:
            fh.write(msg + "\n")
    except OSError:
        pass


def _dbg(msg):
    print(msg, file=sys.stderr, flush=True)
    _logline(msg)



# ---------------------------------------------------------------------------
# Physical-UART frame router: shared by both platform bridges.
# ---------------------------------------------------------------------------
class FrameRouter:
    """Frames channel data onto the real UART and de-frames the reverse path."""

    def __init__(self, uart, channels, max_payload, deliver, debug=False):
        self.uart = uart
        self.channels = channels
        self.max_payload = max_payload
        self.deliver = deliver            # callable(channel:int, payload:bytes)
        self.debug = debug
        self._rx = bytearray()
        self._wlock = threading.Lock()

    def send(self, channel, data):
        # Direction: PC (virtual port) -> MCU. Data written by an application
        # into a virtual port is framed here and pushed onto the real UART.
        if self.debug:
            _dbg(f"[PC->MCU] ch={channel} {len(data)} bytes "
                 f"from virtual port: {data!r}")
        for i in range(0, len(data), self.max_payload):
            chunk = data[i:i + self.max_payload]
            frame = build_frame(channel, chunk)
            if self.debug:
                _dbg(f"[PC->MCU]   ch={channel} framed chunk "
                     f"{len(chunk)} bytes -> {len(frame)} bytes on UART: "
                     f"{frame.hex()}")
            with self._wlock:
                self.uart.write(frame)

    def feed_uart(self, data):
        # Direction: MCU -> PC. Raw bytes read from the real UART are de-framed
        # here and delivered to the matching virtual port.
        if not data:
            return
        if self.debug:
            _dbg(f"[MCU->PC] raw {len(data)} bytes from UART: {data.hex()}")

        self._rx.extend(data)
        while True:
            try:
                zero = self._rx.index(0)
            except ValueError:
                if self.debug and self._rx:
                    _dbg(f"[MCU->PC]   partial frame buffered "
                         f"({len(self._rx)} bytes, no 0x00 yet)")
                break
            body = bytes(self._rx[:zero])
            del self._rx[:zero + 1]
            if self.debug:
                _dbg(f"[MCU->PC]   frame body {len(body)} bytes: {body.hex()}")
            if not body:
                if self.debug:
                    _dbg("[MCU->PC]   DROPPED: empty frame body (stray 0x00)")
                continue
            try:
                decoded = cobs_decode(body)
            except ValueError as exc:
                if self.debug:
                    _dbg(f"[MCU->PC]   DROPPED: COBS decode failed: {exc}")
                continue                  # corrupt frame; resync on next 0x00
            if not decoded:
                if self.debug:
                    _dbg("[MCU->PC]   DROPPED: empty decode")
                continue
            ch = decoded[0]
            payload = decoded[1:]
            if self.debug:
                _dbg(f"[MCU->PC]   decoded ch={ch} payload={len(payload)} "
                     f"bytes: {payload!r}")
            if 0 <= ch < self.channels and payload:
                self.deliver(ch, payload)
                if self.debug:
                    _dbg(f"[MCU->PC]   -> delivered to virtual port "
                         f"channel {ch}")
            elif self.debug:
                if not (0 <= ch < self.channels):
                    _dbg(f"[MCU->PC]   DROPPED: channel {ch} out of range "
                         f"[0, {self.channels})")
                else:
                    _dbg("[MCU->PC]   DROPPED: empty payload")



# ---------------------------------------------------------------------------
# Linux / macOS: real PTY virtual ports exposed as stable symlinks.
# ---------------------------------------------------------------------------
class PosixVPorts:
    def __init__(self, port, baud, channels, vport_names, max_payload,
                 debug=False):
        import pty
        import select
        try:
            import serial  # pyserial
        except ImportError:
            raise SystemExit(
                "pyserial is not installed for this Python interpreter "
                f"({sys.executable}).\n"
                "  Install it with:   python3 -m pip install pyserial\n"
                "  If you run this under sudo, install it for root too:\n"
                "                     sudo python3 -m pip install pyserial\n"
                "  (or re-run with 'sudo -E' to keep your environment).")
        import termios
        import tty


        self._pty = pty
        self._select = select
        self._termios = termios
        self._tty = tty

        self.channels = channels
        self.debug = debug
        # Open the UART with a small BLOCKING timeout (not timeout=0). A
        # dedicated reader thread (see run()) blocks in the driver on this
        # port and is woken as soon as the driver delivers bytes (including
        # when a USB-serial adapter's latency timer expires). A pure
        # non-blocking read (timeout=0) polled from the vport select loop
        # returns only bytes the driver has ALREADY posted to the host, so on
        # some USB-UART adapters the RX FIFO sits undelivered until an
        # unrelated bus transaction (e.g. the host writing a byte after the
        # user types) flushes it - which looked like the MCU boot banner only
        # appearing after a keystroke. A blocking reader removes that coupling.
        self.uart = serial.Serial(port, baud, timeout=0.05)
        self.uart_fd = self.uart.fileno()
        self._stop = threading.Event()


        self.master_fds = []
        self.slave_fds = []
        self.links = []
        for ch in range(channels):
            master_fd, slave_fd = pty.openpty()
            self._make_raw(slave_fd)
            slave_name = os.ttyname(slave_fd)
            link = vport_names[ch]
            try:
                if os.path.islink(link) or os.path.exists(link):
                    os.remove(link)
            except OSError:
                pass
            try:
                os.symlink(slave_name, link)
            except PermissionError:
                os.close(slave_fd)
                for done in self.links:
                    try:
                        os.remove(done)
                    except OSError:
                        pass
                raise SystemExit(
                    f"Permission denied creating the virtual-port symlink "
                    f"'{link}'.\n"
                    "  The default name places it next to the real device "
                    "(e.g. /dev/ttyUSB0CH0), and writing into /dev needs "
                    "root.\n"
                    "  Fix (no root needed): put the symlinks in a writable "
                    "directory, e.g.\n"
                    f"      python3 {os.path.basename(sys.argv[0])} --port "
                    f"{port} --channels {channels} --link-prefix /tmp/vcom\n"
                    "  or name them explicitly with --vport-names "
                    "/tmp/ttyA,/tmp/ttyB,...\n"
                    "  Alternatively re-run as root (sudo), but then pyserial "
                    "must be installed for root too.")
            # Keep the PTY slave fd OPEN for the lifetime of the bridge.
            # If we close it here, no process holds the slave, so Linux marks
            # the master end as hung up (POLLHUP): select() returns it
            # immediately readable and the following os.read() raises
            # OSError: [Errno 5] EIO. That killed every _vport_loop thread at
            # startup, which is exactly why PC->MCU input was never captured
            # (no [PC->MCU] log) even though the bridge started cleanly.
            self.slave_fds.append(slave_fd)

            self.master_fds.append(master_fd)
            self.links.append(link)
            print(f"channel {ch}: {link} -> {slave_name}")

        self.router = FrameRouter(self.uart, channels, max_payload,
                                  self._deliver_to_vport, debug)

    def _make_raw(self, fd):
        termios, tty = self._termios, self._tty
        try:
            tty.setraw(fd)
        except termios.error:
            pass

    def _deliver_to_vport(self, ch, payload):
        try:
            os.write(self.master_fds[ch], payload)
        except OSError:
            pass

    def _uart_loop(self):
        # Dedicated MCU->PC reader thread. It blocks in the driver on the UART
        # (opened with a small non-zero timeout) and is woken the instant the
        # driver delivers bytes, including when a USB-serial adapter's latency
        # timer expires. This decouples the MCU->PC path from any PC->MCU write
        # transaction: previously the UART was drained only from the vport
        # select() loop with a timeout=0 non-blocking read, so on some USB-UART
        # adapters received bytes (e.g. a boot banner) sat in the adapter RX
        # FIFO undelivered until an unrelated bus transaction (the host writing
        # a byte after the user typed) flushed them - which looked like the
        # banner only appearing after a keystroke.
        while not self._stop.is_set():
            try:
                data = self.uart.read(4096)
            except Exception:
                break
            if data:
                self.router.feed_uart(data)

    def _vport_loop(self, ch):
        # Dedicated PC->MCU reader thread for one virtual port. Blocks in
        # select() on this channel's master fd and frames anything an
        # application writes into the vport onto the real UART.
        select = self._select
        fd = self.master_fds[ch]
        while not self._stop.is_set():
            try:
                rlist, _, _ = select.select([fd], [], [], 0.2)
            except (OSError, ValueError):
                break
            if not rlist:
                continue
            try:
                data = os.read(fd, 4096)
            except OSError:
                # Transient read error (e.g. a client closing its end): do not
                # kill the reader thread. Sleep briefly to avoid a busy spin
                # if the condition persists, then re-select.
                time.sleep(0.05)
                continue
            if data:
                self.router.send(ch, data)

    def run(self):
        print(f"\nBridge running on {self.uart.port} @ {self.uart.baudrate} "
              f"baud. Open the ports above from any tool. Ctrl-C to stop.\n")
        if self.debug:
            _dbg(f"[debug] RX instrumentation ON, channels=0..{self.channels - 1}")
        # One blocking UART reader thread (MCU->PC) plus one select() thread
        # per virtual port (PC->MCU). The blocking reader guarantees the
        # adapter's RX FIFO is drained as soon as bytes arrive, without waiting
        # for a write transaction or a poll-loop cadence.
        threads = [threading.Thread(target=self._uart_loop, daemon=True)]
        for ch in range(self.channels):
            threads.append(threading.Thread(target=self._vport_loop,
                                            args=(ch,), daemon=True))
        for t in threads:
            t.start()
        try:
            while True:
                for t in threads:
                    t.join(0.3)
        except KeyboardInterrupt:
            print("\nStopping.")
        finally:
            self._stop.set()
            self._cleanup()

    def _cleanup(self):
        self._stop.set()

        for fd in self.master_fds:
            try:
                os.close(fd)
            except OSError:
                pass
        for fd in getattr(self, "slave_fds", []):
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


# ---------------------------------------------------------------------------
# Windows com0com automation: locate / install the driver and create the
# virtual port PAIRS so third-party software can just open the app-side name.
# ---------------------------------------------------------------------------
# Signed com0com build with a Windows 10/11 compatible driver.
COM0COM_URL = ("https://download.pololu.com/file/0J1602/"
               "com0com-3.0.0.0-i386-and-x64-signed.zip")

# Installer download URLs for the signed third-party tools. These can be
# overridden at runtime with --vsp-installer-url or the environment variable
# SERIAL_MUX_VSP_INSTALLER_URL (useful behind a corporate mirror, or to point
# at a licensed Eltima build you already downloaded). Eltima's "Virtual Serial
# Port Driver" is commercial software: the URL below is its official installer,
# but a valid license/registration is still required for it to run unrestricted
# -- this only automates the DOWNLOAD + SILENT INSTALL step, not licensing.
ELTIMA_URL = "https://www.eltima.com/download/vspd/"
# HHD "Free Virtual Serial Ports" installer (free, signed, but GUI-only: the
# pairs still cannot be scripted after install).
HHD_URL = ("https://freevirtualserialports.com/download/"
           "free-virtual-serial-port-tools.zip")



def _find_setupc():
    """Return the path to com0com's setupc.exe, or None if not installed."""
    candidates = []
    for env in ("ProgramFiles(x86)", "ProgramFiles", "ProgramW6432"):
        base = os.environ.get(env)
        if base:
            candidates.append(os.path.join(base, "com0com", "setupc.exe"))
    for path in candidates:
        if os.path.isfile(path):
            return path
    return None


def _run_setupc(setupc, args):
    """Run setupc.exe with the given argument list; return (rc, output)."""
    cmd = [setupc] + args
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True,
                              cwd=os.path.dirname(setupc), timeout=60)
        return proc.returncode, (proc.stdout or "") + (proc.stderr or "")
    except Exception as exc:  # noqa: BLE001
        return 1, str(exc)


def _existing_com_names(setupc):
    """Return the set of COM names already in use / reserved on the system.

    Combines what Windows currently enumerates (pyserial's list_ports) with
    every explicit PortName com0com already has configured (from 'setupc
    list'), so we never hand out a number that will collide.
    """
    used = set()
    try:
        from serial.tools import list_ports
        for p in list_ports.comports():
            if p.device and p.device.upper().startswith("COM"):
                used.add(p.device.upper())
    except Exception:  # noqa: BLE001
        pass
    rc, out = _run_setupc(setupc, ["list"])
    if rc == 0:
        for line in out.splitlines():
            if "PortName=" not in line:
                continue
            pn = line.split("PortName=", 1)[1].strip().upper()
            if pn.startswith("COM") and "#" not in pn and pn != "COM":
                used.add(pn)
    return used


def _alloc_free_com_names(setupc, count, start=20):
    """Allocate `count` free, explicit COMxx names not currently in use."""
    used = _existing_com_names(setupc)
    names = []
    n = start
    while len(names) < count:
        cand = f"COM{n}"
        if cand not in used:
            names.append(cand)
            used.add(cand)
        n += 1
        if n > 256:
            raise SystemExit("could not find enough free COM numbers "
                             "(searched COM20..COM256).")
    return names



def _install_com0com():
    """Download and silently install com0com. Returns setupc.exe path or None.

    Requires Administrator rights (driver install). If not elevated, the
    installer will prompt / fail and we return None with guidance.
    """
    import tempfile
    import zipfile

    print("com0com not found. Downloading the installer ...")
    tmp = tempfile.mkdtemp(prefix="com0com_")
    zip_path = os.path.join(tmp, "com0com.zip")
    try:
        urllib.request.urlretrieve(COM0COM_URL, zip_path)
    except Exception as exc:  # noqa: BLE001
        print(f"  download failed: {exc}")
        return None

    exe = None
    try:
        with zipfile.ZipFile(zip_path) as zf:
            zf.extractall(tmp)
    except Exception as exc:  # noqa: BLE001
        print(f"  unzip failed: {exc}")
        return None
    for root, _dirs, files in os.walk(tmp):
        for name in files:
            if name.lower().startswith("setup") and name.lower().endswith(".exe"):
                exe = os.path.join(root, name)
                break
        if exe:
            break
    if not exe:
        print("  installer executable not found in the archive.")
        return None

    print(f"Running silent install: {exe}")
    # com0com's Inno Setup installer supports /S (silent) and installs the
    # signed driver. This needs Administrator rights.
    rc, out = 1, ""
    try:
        proc = subprocess.run([exe, "/S"], capture_output=True, text=True,
                              timeout=300)
        rc, out = proc.returncode, (proc.stdout or "") + (proc.stderr or "")
    except Exception as exc:  # noqa: BLE001
        print(f"  install failed: {exc}")
        return None
    if rc != 0:
        print(f"  installer returned {rc}: {out}")
    time.sleep(3)
    return _find_setupc()


def ensure_com0com_pairs(channels, real_port):
    """Ensure `channels` com0com pairs exist and return the bridge-side ports.

    App-side ports are named "<real_port>CHn" (e.g. COM3CH0); the paired
    bridge-side ports keep auto-assigned CNCBn names, which this bridge opens.
    Installs com0com automatically if it is missing (Administrator needed).
    """
    setupc = _find_setupc()
    if not setupc:
        setupc = _install_com0com()
    if not setupc:
        raise SystemExit(
            "com0com is not installed and automatic install failed "
            "(no network access to the installer is the usual cause).\n"
            "\n"
            "Windows cannot create a virtual COM port from user space, so a\n"
            "signed com0com kernel driver is required. Do ONE of the following:\n"
            "\n"
            "OPTION A - install com0com once, then re-run (auto-creates pairs):\n"
            "  1. On any machine with internet, download the signed build:\n"
            "       https://com0com.sourceforge.net\n"
            "       (or the bundled mirror: %s )\n"
            "  2. Copy it over and run setup.exe AS ADMINISTRATOR to install.\n"
            "  3. In an ADMINISTRATOR shell, re-run this bridge, e.g.:\n"
            "       python serial_mux_vports.py --port %s --channels %d\n"
            "     It will create %sCH0..%sCH%d for your terminal apps.\n"
            "\n"
            "OPTION B - you already made com0com pairs yourself:\n"
            "  Pass the BRIDGE-side port of each pair (apps open the OTHER side),\n"
            "  and skip the auto step, e.g. for 2 pairs COM20/COM21, COM22/COM23:\n"
            "       python serial_mux_vports.py --port %s --channels %d \\\n"
            "           --win-bridge-ports COM21,COM23 --no-auto-com0com\n"
            % (COM0COM_URL, real_port, channels, real_port, real_port,
               channels - 1, real_port, channels))


    # Pre-allocate one explicit, free COMxx name per channel for the bridge
    # side. com0com's "PortName=COM#" auto-assign token is stored verbatim and
    # 'setupc list' reports the literal string "COM#" (not a resolved COMx),
    # which pyserial then cannot open ("could not open port 'COM#'"). Assigning
    # an explicit COMxx we picked from the free pool avoids that entirely.
    free_names = _alloc_free_com_names(setupc, channels)

    bridge_ports = []
    for ch in range(channels):
        app_name = f"{real_port}CH{ch}"
        bridge_name = free_names[ch]
        # Give BOTH sides a real COM name: the app side the friendly
        # "<real_port>CHn" name, and the bridge side an explicit, free COMxx.
        rc, out = _run_setupc(setupc,
                              ["install",
                               f"PortName={app_name}",
                               f"PortName={bridge_name}"])


        if rc != 0:
            # Creating a com0com pair writes to the kernel driver, which needs
            # Administrator rights. A non-elevated shell fails with WinError
            # 740 ("The requested operation requires elevation"). Detect that
            # and stop with a clear instruction instead of the confusing
            # "could not resolve its bridge-side port" message below.
            low = out.lower()
            if "740" in out or "requires elevation" in low or "elevation" in low:
                raise SystemExit(
                    "Creating com0com virtual ports requires Administrator "
                    "rights (WinError 740).\n"
                    "\n"
                    "Fix: close this window, then open PowerShell or cmd AS "
                    "ADMINISTRATOR\n"
                    "(right-click > 'Run as administrator') and re-run:\n"
                    f"    python serial_mux_vports.py --port {real_port} "
                    f"--channels {channels}\n"
                    "\n"
                    "If a half-created pair was left behind, clean it up first "
                    "(also in the\n"
                    "elevated shell):\n"
                    f"    python serial_mux_vports.py --port {real_port} "
                    f"--channels {channels} --remove\n")
            print(f"  warning: creating pair for {app_name} returned {rc}: {out}")
        # Discover the bridge-side of the pair we just created.
        bridge = _bridge_side_for(setupc, app_name)
        if not bridge:
            raise SystemExit(
                f"created a pair for {app_name} but could not resolve its "
                f"bridge-side port. Run '{setupc} list' to inspect.")

        # If com0com left the bridge side as its internal alias (CNCAn/CNCBn)
        # instead of a real COMx, pyserial cannot open it ("could not open
        # port 'CNCB2'"). Force a real, auto-assigned COM number with
        # 'setupc change <alias> PortName=COM#' and re-resolve. This is the
        # robust fix regardless of whether the install-time PortName=COM#
        # argument was honored.
        if bridge.upper().startswith(("CNCA", "CNCB")):
            alias = bridge
            rc, out = _run_setupc(setupc,
                                  ["change", alias, "PortName=COM#"])
            low = out.lower()
            if rc != 0 and ("740" in out or "elevation" in low):
                raise SystemExit(
                    "Assigning a real COM number to the com0com bridge side "
                    "requires Administrator rights (WinError 740).\n"
                    "Open PowerShell/cmd AS ADMINISTRATOR and re-run:\n"
                    f"    python serial_mux_vports.py --port {real_port} "
                    f"--channels {channels}\n")
            if rc != 0:
                print(f"  warning: 'change {alias} PortName=COM#' returned "
                      f"{rc}: {out}")
            # Re-resolve the bridge side; it should now be a real COMx.
            bridge = _bridge_side_for(setupc, app_name) or bridge
            if bridge.upper().startswith(("CNCA", "CNCB")):
                raise SystemExit(
                    f"the bridge side of {app_name} is still the internal "
                    f"alias '{bridge}', which pyserial cannot open.\n"
                    f"Assign it a real COM name manually (as Administrator):\n"
                    f"    \"{setupc}\" change {alias} PortName=COM#\n"
                    f"then re-run. Inspect with:  \"{setupc}\" list")

        bridge_ports.append(bridge)
        print(f"channel {ch}: app opens {app_name}  (bridge side {bridge})")

    # com0com's 'setupc install' registers the pair in the driver database,
    # but on Windows 10/11 the actual COM device nodes (\\.\COMxx) are often
    # NOT spawned until the bus driver is told to re-enumerate. Without this,
    # 'setupc list' happily reports PortName=COM20 while serial.Serial('COM20')
    # keeps failing with FileNotFoundError(2) ("system cannot find the file")
    # because the device node simply does not exist yet. Force a driver
    # re-enumeration and then wait for each name to actually appear in the
    # OS port list before returning.
    _reload_com0com(setupc)
    _wait_for_ports(bridge_ports)
    return bridge_ports


def _reload_com0com(setupc):
    """Ask the com0com bus driver to re-enumerate so port nodes get created.

    Different com0com builds expose this as 'update' or 'reload'; try both and
    ignore failures (the subsequent presence check is the real gate).
    """
    for verb in ("update", "reload"):
        rc, out = _run_setupc(setupc, [verb])
        if rc == 0:
            print(f"  com0com '{verb}' issued to enumerate the new ports")
            return
    print("  warning: 'setupc update'/'reload' did not succeed; the ports may "
          "take longer to appear")


def _wait_for_ports(names, attempts=40, delay=0.25):
    r"""Block until every COM name in `names` is enumerated by Windows.

    Uses pyserial's list_ports, which reflects the real OS device nodes (not
    just com0com's own database), so this only returns once \\.\COMxx actually
    exists and serial.Serial(name) can succeed.
    """

    try:
        from serial.tools import list_ports
    except Exception:  # noqa: BLE001
        time.sleep(delay * 4)          # no way to verify; give PnP a moment
        return
    want = {n.upper() for n in names}
    for i in range(attempts):
        have = {p.device.upper() for p in list_ports.comports()
                if p.device}
        missing = want - have
        if not missing:
            return
        if i == 0:
            print(f"  waiting for {sorted(missing)} to enumerate "
                  f"(com0com PnP settle) ...")
        time.sleep(delay)
    # Fall through: let the open-with-retry produce the guided error if the
    # node still never appeared.




def remove_com0com_pairs(channels, real_port):
    """Delete the com0com pairs this script created (<real_port>CHn app side).

    Parses 'setupc list', finds every pair whose app side is named
    "<real_port>CH0".."<real_port>CH{channels-1}", and runs
    'setupc remove <pairnum>' for each. Needs Administrator rights.
    Returns the number of pairs removed.
    """
    setupc = _find_setupc()
    if not setupc:
        raise SystemExit(
            "com0com is not installed, so there is nothing to remove.\n"
            "If you created ports manually, remove them with com0com's "
            "setupc.exe (run as Administrator).")

    wanted = {f"{real_port}CH{ch}" for ch in range(channels)}
    rc, out = _run_setupc(setupc, ["list"])
    if rc != 0:
        raise SystemExit(f"'{setupc} list' failed: {out}")

    # Map pair number -> app-side portname, for CNCA/CNCB lines that match.
    pairs_to_remove = set()
    for line in out.splitlines():
        line = line.strip()
        if not line or "PortName=" not in line:
            continue
        ident = line.split()[0]                     # e.g. CNCA0 / CNCB3
        pn = line.split("PortName=", 1)[1].strip()
        if pn in wanted and (ident.startswith("CNCA") or
                             ident.startswith("CNCB")):
            num = ident[len("CNCA"):] if ident.startswith("CNCA") \
                else ident[len("CNCB"):]
            pairs_to_remove.add(num)

    if not pairs_to_remove:
        print(f"No com0com pairs named {sorted(wanted)} were found.")
        return 0

    removed = 0
    for num in sorted(pairs_to_remove, key=lambda x: int(x) if x.isdigit()
                      else x):
        rc, out = _run_setupc(setupc, ["remove", num])
        if rc != 0:
            print(f"  warning: 'remove {num}' returned {rc}: {out}")
        else:
            print(f"removed com0com pair {num}")
            removed += 1
    return removed


# ---------------------------------------------------------------------------
# Non-com0com signed alternatives: Eltima "Virtual Serial Port Driver"
# (WHQL-signed, CLI-automatable via vspdctl.exe) and HHD Software "Free
# Virtual Serial Ports" (attestation-signed, GUI-only -> guided fallback).
#
# These are for machines where the com0com driver fails to load with Code 52
# (signature-not-trusted), which no code change can fix. Both tools create the
# same PAIR model: an APP-side COM that third-party software opens, and a
# BRIDGE-side COM that this script opens. ensure_eltima_pairs() automates the
# pair creation; ensure_hhd_pairs() prints guided steps and requires the user
# to pass the pre-created bridge ports via --win-bridge-ports.
# ---------------------------------------------------------------------------
def _download_installer(url, prefix):
    """Download an installer from `url` to a temp file. Returns path or None.

    Handles both direct .exe/.msi/.zip links and simple redirect pages. The
    saved filename keeps the URL's extension when present, else defaults to
    .exe so we can execute it.
    """
    import tempfile
    from urllib.parse import urlparse

    tmp = tempfile.mkdtemp(prefix=prefix)
    name = os.path.basename(urlparse(url).path) or "installer.exe"
    if not os.path.splitext(name)[1]:
        name += ".exe"
    dest = os.path.join(tmp, name)
    print(f"Downloading installer from {url} ...")
    try:
        req = urllib.request.Request(
            url, headers={"User-Agent": "Mozilla/5.0 serial_mux_vports"})
        with urllib.request.urlopen(req, timeout=120) as resp, \
                open(dest, "wb") as fh:
            fh.write(resp.read())
    except Exception as exc:  # noqa: BLE001
        print(f"  download failed: {exc}")
        return None
    return dest


def _run_installer_silent(installer, silent_flags):
    """Run a downloaded installer with the given silent flags; return rc.

    Tries each flag string in `silent_flags` until one exits 0. This needs
    Administrator rights for a driver install. Returns the last return code.
    """
    if installer.lower().endswith(".zip"):
        import zipfile
        tmp = os.path.dirname(installer)
        try:
            with zipfile.ZipFile(installer) as zf:
                zf.extractall(tmp)
        except Exception as exc:  # noqa: BLE001
            print(f"  unzip failed: {exc}")
            return 1
        exe = None
        for root, _dirs, files in os.walk(tmp):
            for fn in files:
                low = fn.lower()
                if low.startswith("setup") and low.endswith(".exe"):
                    exe = os.path.join(root, fn)
                    break
            if exe:
                break
        if not exe:
            print("  no setup*.exe found inside the archive.")
            return 1
        installer = exe

    rc = 1
    for flags in silent_flags:
        args = [installer] + (flags.split() if flags else [])
        try:
            print(f"Running installer: {' '.join(args)}")
            proc = subprocess.run(args, capture_output=True, text=True,
                                  timeout=600)
            rc = proc.returncode
            if rc == 0:
                break
            print(f"  installer returned {rc}: "
                  f"{(proc.stdout or '') + (proc.stderr or '')}")
        except Exception as exc:  # noqa: BLE001
            print(f"  install attempt failed: {exc}")
            rc = 1
    time.sleep(3)
    return rc


def _installer_url_for(tool, default):
    """Resolve the installer URL for `tool`, honoring CLI/env overrides."""
    override = (os.environ.get("SERIAL_MUX_VSP_INSTALLER_URL") or
                globals().get("_VSP_INSTALLER_URL_OVERRIDE"))
    return override or default


def _install_eltima():
    """Download and silently install Eltima VSPD. Returns vspdctl path or None.

    NOTE: Eltima 'Virtual Serial Port Driver' is COMMERCIAL software. This only
    automates the download + silent install of the official installer; a valid
    license is still required for the driver to operate unrestricted. Provide a
    licensed/mirror installer with --vsp-installer-url if the default download
    is not usable in your environment. Needs Administrator rights.
    """
    url = _installer_url_for("eltima", ELTIMA_URL)
    installer = _download_installer(url, "eltima_vspd_")
    if not installer:
        return None
    # Eltima uses an InstallShield/Inno-style installer; try common silent
    # switches in order.
    _run_installer_silent(installer, ["/S", "/silent", "/verysilent",
                                      "/quiet", ""])
    return _find_vspdctl()


def _install_hhd():
    """Download and silently install HHD 'Free Virtual Serial Ports'.

    HHD is free and signed, but GUI-only: even after this installs it, the
    port PAIRS cannot be created from a CLI. Returns True if the installer ran.
    Needs Administrator rights.
    """
    url = _installer_url_for("hhd", HHD_URL)
    installer = _download_installer(url, "hhd_vsp_")
    if not installer:
        return False
    rc = _run_installer_silent(installer, ["/S", "/silent", "/verysilent",
                                           "/quiet", ""])
    return rc == 0


def _find_vspdctl():
    """Return the path to Eltima's vspdctl.exe, or None if not installed."""

    candidates = []
    for env in ("ProgramFiles(x86)", "ProgramFiles", "ProgramW6432"):
        base = os.environ.get(env)
        if not base:
            continue
        for sub in ("Eltima Software",
                    os.path.join("Eltima Software",
                                 "Virtual Serial Port Driver"),
                    "Virtual Serial Port Driver",
                    os.path.join("Electronic Team",
                                 "Virtual Serial Port Driver")):
            candidates.append(os.path.join(base, sub, "vspdctl.exe"))
    for path in candidates:
        if os.path.isfile(path):
            return path
    # Fall back to PATH (some installs register vspdctl there).
    from shutil import which
    return which("vspdctl.exe") or which("vspdctl")


def _run_vspdctl(vspdctl, args):
    """Run vspdctl.exe with the given argument list; return (rc, output)."""
    cmd = [vspdctl] + args
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True,
                              cwd=os.path.dirname(vspdctl) or None, timeout=60)
        return proc.returncode, (proc.stdout or "") + (proc.stderr or "")
    except Exception as exc:  # noqa: BLE001
        return 1, str(exc)


def _existing_com_names_generic():
    """Return the set of COM names currently enumerated by Windows."""
    used = set()
    try:
        from serial.tools import list_ports
        for p in list_ports.comports():
            if p.device and p.device.upper().startswith("COM"):
                used.add(p.device.upper())
    except Exception:  # noqa: BLE001
        pass
    return used


def _alloc_free_com_names_generic(count, used, start=20):
    """Allocate `count` free COMxx names not present in `used`."""
    names = []
    n = start
    while len(names) < count:
        cand = f"COM{n}"
        if cand not in used:
            names.append(cand)
            used.add(cand)
        n += 1
        if n > 256:
            raise SystemExit("could not find enough free COM numbers "
                             "(searched COM20..COM256).")
    return names


def ensure_eltima_pairs(channels, real_port):
    """Ensure `channels` Eltima VSPD pairs exist; return the bridge-side ports.

    Uses Eltima's CLI:  vspdctl.exe add <appCOM> <bridgeCOM>
    App side is <appCOM> (opened by third-party software); bridge side is the
    port this script opens. Needs Administrator rights (driver operation).
    """
    vspdctl = _find_vspdctl()
    if not vspdctl:
        # Best-effort auto-download + silent install (mirrors _install_com0com).
        print("Eltima vspdctl.exe not found; attempting auto-download+install ...")
        vspdctl = _install_eltima()
    if not vspdctl:
        raise SystemExit(
            "Eltima 'Virtual Serial Port Driver' (vspdctl.exe) was not found.\n"
            "\n"
            "This is the recommended fix when com0com's driver fails to load\n"
            "with Code 52 (signature-not-trusted) on Windows 10/11, because\n"
            "Eltima ships a WHQL-signed driver with an automatable CLI.\n"
            "\n"
            "  1. Install 'Virtual Serial Port Driver' from eltima.com\n"
            "     (a.k.a. Electronic Team Virtual Serial Port Driver).\n"
            "  2. Re-run this bridge in an ADMINISTRATOR shell:\n"
            "       python serial_mux_vports.py --port %s --channels %d "
            "--vsp-tool eltima\n"
            "\n"
            "If you prefer a free signed tool, use HHD 'Free Virtual Serial\n"
            "Ports' (GUI-only): create the pairs in its UI, then pass the\n"
            "bridge-side ports yourself:\n"
            "       python serial_mux_vports.py --port %s --channels %d "
            "--vsp-tool hhd\n"
            % (real_port, channels, real_port, channels))

    # Allocate explicit, free COM names for BOTH sides so nothing collides
    # with existing ports (including the real UART).
    used = _existing_com_names_generic()
    used.add(real_port.upper())
    names = _alloc_free_com_names_generic(channels * 2, used)
    app_names = names[0::2]
    bridge_names = names[1::2]

    bridge_ports = []
    for ch in range(channels):
        app_name = app_names[ch]
        bridge_name = bridge_names[ch]
        rc, out = _run_vspdctl(vspdctl, ["add", app_name, bridge_name])
        low = out.lower()
        if rc != 0:
            if "740" in out or "elevation" in low or "administrator" in low:
                raise SystemExit(
                    "Creating Eltima virtual ports requires Administrator "
                    "rights.\n"
                    "Open PowerShell/cmd AS ADMINISTRATOR and re-run:\n"
                    f"    python serial_mux_vports.py --port {real_port} "
                    f"--channels {channels} --vsp-tool eltima\n")
            print(f"  warning: 'vspdctl add {app_name} {bridge_name}' "
                  f"returned {rc}: {out}")
        bridge_ports.append(bridge_name)
        print(f"channel {ch}: app opens {app_name}  (bridge side {bridge_name})")

    # Wait for the OS to enumerate the freshly created device nodes so the
    # subsequent serial.Serial(bridge) open does not race PnP.
    _wait_for_ports(bridge_ports)
    return bridge_ports


def remove_eltima_pairs(channels, real_port):
    """Delete Eltima VSPD pairs previously created by this script.

    Best-effort: parse 'vspdctl list' and delete every pair whose ports are in
    the COM20+ range this script hands out. Needs Administrator rights.
    """
    vspdctl = _find_vspdctl()
    if not vspdctl:
        raise SystemExit(
            "Eltima 'Virtual Serial Port Driver' (vspdctl.exe) is not "
            "installed, so there is nothing to remove.")
    rc, out = _run_vspdctl(vspdctl, ["list"])
    if rc != 0:
        raise SystemExit(f"'vspdctl list' failed: {out}")
    # Collect COM pairs reported by vspdctl (format varies by version; match
    # any two COM names on a line).
    import re
    removed = 0
    seen = set()
    for line in out.splitlines():
        coms = re.findall(r"COM\d+", line.upper())
        if len(coms) >= 2:
            a, b = coms[0], coms[1]
            if a in seen or b in seen:
                continue
            rc2, out2 = _run_vspdctl(vspdctl, ["remove", a, b])
            if rc2 != 0:
                rc2, out2 = _run_vspdctl(vspdctl, ["delete", a, b])
            if rc2 == 0:
                print(f"removed Eltima pair {a}/{b}")
                seen.update((a, b))
                removed += 1
            else:
                print(f"  warning: could not remove {a}/{b}: {out2}")
    if not removed:
        print("No Eltima pairs were removed (nothing matched, or use the "
              "Eltima GUI to delete them).")
    return removed


def ensure_hhd_pairs(channels, real_port, bridge_ports):
    """Validate/return HHD-created bridge ports; HHD has no reliable CLI.

    HHD 'Free Virtual Serial Ports' is signed but GUI-only, so the pairs must
    be created by hand in its UI and their bridge-side ports supplied via
    --win-bridge-ports. This helper just enforces that and returns them.
    """
    if not bridge_ports:
        # Best-effort: auto-download+install HHD if it is not present, then
        # still require the user to create the pairs in its GUI (HHD has no
        # scriptable CLI, so we cannot make the pairs ourselves).
        if not _find_vspdctl():   # HHD has no CLI; only install if nothing else
            print("Attempting auto-download+install of HHD 'Free Virtual "
                  "Serial Ports' ...")
            if _install_hhd():
                print("HHD installed. Now create the pairs in its GUI, then "
                      "re-run with --win-bridge-ports.")
        raise SystemExit(
            "HHD 'Free Virtual Serial Ports' is GUI-only (no automatable "
            "CLI), so this script cannot create the pairs for you.\n"
            "\n"
            "Do this once in the HHD 'Free Virtual Serial Ports' window:\n"
            f"  1. Create {channels} 'Local Bridge' pair(s). Each pair gives\n"
            "     you two COM ports (an app side and a bridge side).\n"
            "  2. Note the BRIDGE-side COM of each pair.\n"
            "  3. Re-run this bridge, passing those bridge ports, e.g.:\n"
            f"       python serial_mux_vports.py --port {real_port} "
            f"--channels {channels} \\\n"
            "           --vsp-tool hhd --win-bridge-ports COM21,COM23,...\n"
            "\n"
            "Your terminal software then opens the OTHER (app) side of each "
            "pair.")
    if len(bridge_ports) != channels:
        raise SystemExit(
            f"--win-bridge-ports needs exactly {channels} port names "
            f"(got {len(bridge_ports)}).")
    _wait_for_ports(bridge_ports)
    return bridge_ports


def remove_posix_vports(names):
    """Delete leftover POSIX PTY symlinks (dangling after an abrupt kill)."""
    removed = 0
    for link in names:
        try:
            if os.path.islink(link) or os.path.exists(link):
                os.remove(link)
                print(f"removed {link}")
                removed += 1
        except OSError as exc:  # noqa: BLE001
            print(f"  warning: could not remove {link}: {exc}")
    if not removed:
        print("No matching virtual-port symlinks were found.")
    return removed


def _bridge_side_for(setupc, app_name):

    """Parse 'setupc list' to find the bridge-side port paired with app_name.

    com0com lists each pair as two CNCAn/CNCBn lines sharing a pair number:
        CNCA0 PortName=COM3CH0
        CNCB0 PortName=COM21          (or PortName=- meaning use CNCB0)
    We locate the pair whose app side matches app_name and return the other
    side's usable name (its explicit PortName, else its CNCBn alias).
    """
    rc, out = _run_setupc(setupc, ["list"])
    if rc != 0:
        return None
    entries = {}   # ident (e.g. CNCA0) -> portname string
    for line in out.splitlines():
        line = line.strip()
        if not line or "PortName=" not in line:
            continue
        ident = line.split()[0]                     # CNCA0 / CNCB0
        pn = line.split("PortName=", 1)[1].strip()
        entries[ident] = pn

    def resolved(ident):
        pn = entries.get(ident, "-")
        return ident if pn == "-" else pn

    for ident, pn in entries.items():
        if pn == app_name and ident.startswith("CNCA"):
            num = ident[len("CNCA"):]
            return resolved("CNCB" + num)
        if pn == app_name and ident.startswith("CNCB"):
            num = ident[len("CNCB"):]
            return resolved("CNCA" + num)
    return None


# ---------------------------------------------------------------------------
# Windows: bridge the com0com "bridge side" ports to the one real UART.
# The application opens the paired "app side" port (e.g. COM3CH0).
# ---------------------------------------------------------------------------
class WindowsVPorts:

    def __init__(self, port, baud, channels, bridge_ports, max_payload,
                 debug=False):
        import serial  # pyserial

        if len(bridge_ports) != channels:
            raise SystemExit(
                f"--win-bridge-ports needs exactly {channels} port names "
                f"(got {len(bridge_ports)}). Create that many com0com pairs.")

        self.channels = channels
        self.debug = debug
        self.uart = serial.Serial(port, baud, timeout=0.05)
        self.vports = []
        for ch, name in enumerate(bridge_ports):
            vp = self._open_with_retry(serial, name, baud)
            self.vports.append(vp)
            print(f"channel {ch}: bridge side {name} "
                  f"(app opens the paired com0com port)")


        self.router = FrameRouter(self.uart, channels, max_payload,
                                  self._deliver_to_vport, debug)
        self._stop = threading.Event()

    @staticmethod
    def _open_with_retry(serial, name, baud, attempts=40, delay=0.25):
        # A com0com pair created moments ago by 'setupc install' is not yet
        # openable: Windows PnP has to finish creating the COM device node,
        # so an immediate serial.Serial(name) raises FileNotFoundError
        # (WinError 2, "The system cannot find the file specified") even
        # though 'setupc list' already reports the name. Retry with a short
        # backoff (default up to ~10s) until PnP finishes enumerating it.
        last_exc = None
        for i in range(attempts):
            try:
                return serial.Serial(name, baud, timeout=0.05)
            except serial.SerialException as exc:
                last_exc = exc
                if i == 0:
                    print(f"  waiting for {name} to enumerate "
                          f"(com0com PnP settle) ...")
                time.sleep(delay)
        raise SystemExit(
            f"could not open the com0com bridge port {name!r} after "
            f"{attempts} attempts (~{attempts * delay:.0f}s):\n"
            f"    {last_exc}\n"
            "\n"
            "The pair was created but Windows never finished enumerating the\n"
            "port. Try:\n"
            "  * unplug/replug or 'setupc update' in an elevated shell, or\n"
            "  * remove and recreate the pairs:\n"
            f"      python serial_mux_vports.py --port COM<real> "
            f"--channels <n> --remove\n"
            "    then re-run.\n"
            "  * inspect current names:  "
            "\"C:\\Program Files (x86)\\com0com\\setupc.exe\" list")

    def _deliver_to_vport(self, ch, payload):

        try:
            self.vports[ch].write(payload)
        except Exception:
            pass

    def _uart_loop(self):
        while not self._stop.is_set():
            try:
                data = self.uart.read(4096)
            except Exception:
                break
            self.router.feed_uart(data)

    def _vport_loop(self, ch):
        vp = self.vports[ch]
        while not self._stop.is_set():
            try:
                data = vp.read(4096)
            except Exception:
                break
            if data:
                self.router.send(ch, data)

    def run(self):
        print(f"\nBridge running on {self.uart.port} @ {self.uart.baudrate} "
              f"baud. Open the paired com0com ports from any tool. "
              f"Ctrl-C to stop.\n")
        if self.debug:
            _dbg(f"[debug] RX instrumentation ON, channels=0..{self.channels - 1}")
        threads = [threading.Thread(target=self._uart_loop, daemon=True)]
        for ch in range(self.channels):
            threads.append(threading.Thread(target=self._vport_loop,
                                            args=(ch,), daemon=True))
        for t in threads:
            t.start()
        try:
            while True:
                for t in threads:
                    t.join(0.3)
        except KeyboardInterrupt:
            print("\nStopping.")
        finally:
            self._stop.set()
            for vp in self.vports:
                try:
                    vp.close()
                except Exception:
                    pass
            try:
                self.uart.close()
            except Exception:
                pass


# ---------------------------------------------------------------------------
def default_posix_names(real_port, channels, prefix):
    if prefix:
        return [f"{prefix}{ch}" for ch in range(channels)]
    # e.g. /dev/ttyUSB0 -> /dev/ttyUSB0CH0 .. (kept in the same directory)
    return [f"{real_port}CH{ch}" for ch in range(channels)]


def main():
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", required=True, help="real serial device (/dev/ttyUSB0, COM3)")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--channels", type=int, default=4)
    ap.add_argument("--max-payload", type=int, default=240)

    # POSIX naming
    ap.add_argument("--link-prefix", default=None,
                    help="POSIX: fixed symlink prefix (creates <prefix>0..N-1). "
                         "If omitted, names are <real_port>CH0..N-1.")
    ap.add_argument("--vport-names", default=None,
                    help="POSIX: explicit comma-separated symlink paths "
                         "(overrides --link-prefix).")

    # Windows com0com
    ap.add_argument("--win-bridge-ports", default=None,
                    help="Windows: comma-separated com0com BRIDGE-side ports, "
                         "one per channel (e.g. COM21,COM23). If omitted, this "
                         "script auto-installs com0com (if missing) and creates "
                         "<real_port>CHn app-side pairs for you.")
    ap.add_argument("--no-auto-com0com", action="store_true",
                    help="Windows: do not auto-install/create com0com pairs; "
                         "require --win-bridge-ports instead.")
    ap.add_argument("--vsp-tool", choices=("com0com", "eltima", "hhd"),
                    default="com0com",
                    help="Windows: which virtual-serial-port tool to use for "
                         "auto pair creation. 'com0com' (default) auto-"
                         "installs/creates com0com pairs. 'eltima' uses "
                         "Eltima 'Virtual Serial Port Driver' (WHQL-signed) "
                         "via its vspdctl.exe CLI - the recommended fix when "
                         "com0com's driver fails to load with Code 52. 'hhd' "
                         "uses HHD 'Free Virtual Serial Ports' (signed, GUI-"
                         "only): create the pairs by hand and pass their "
                         "bridge-side ports via --win-bridge-ports.")
    ap.add_argument("--vsp-installer-url", default=None,
                    help="Windows: override the download URL used to auto-"
                         "install the selected --vsp-tool (eltima/hhd). Useful "
                         "behind a corporate mirror or to point at a licensed "
                         "Eltima installer. Also settable via the "
                         "SERIAL_MUX_VSP_INSTALLER_URL environment variable.")
    ap.add_argument("--debug", action="store_true",
                    help="print per-direction diagnostics to stderr, each line "
                         "tagged [PC->MCU] (data written into a virtual port, "
                         "framed onto the UART) or [MCU->PC] (raw UART bytes "
                         "de-framed and delivered to a virtual port): raw "
                         "bytes, frame body, COBS decode result, decoded "
                         "channel/length, and drop reason)")

    ap.add_argument("--remove", action="store_true",
                    help="tear down the virtual ports created for --port and "
                         "--channels, then exit. Windows: 'setupc remove' each "
                         "<real_port>CHn com0com pair (needs Administrator). "
                         "POSIX: delete leftover PTY symlinks. Does not run the "
                         "bridge.")
    args = ap.parse_args()

    if args.vsp_installer_url:
        globals()["_VSP_INSTALLER_URL_OVERRIDE"] = args.vsp_installer_url

    if args.channels < 1 or args.channels > 250:
        ap.error("--channels must be 1..250")

    if args.remove:
        if sys.platform.startswith("win"):
            if args.vsp_tool == "eltima":
                remove_eltima_pairs(args.channels, args.port)
            elif args.vsp_tool == "hhd":
                print("HHD 'Free Virtual Serial Ports' is GUI-only; remove "
                      "the pairs in its window (this script cannot).")
            else:
                remove_com0com_pairs(args.channels, args.port)
        else:
            if args.vport_names:
                names = [p.strip() for p in args.vport_names.split(",")
                         if p.strip()]
            else:
                names = default_posix_names(args.port, args.channels,
                                            args.link_prefix)
            remove_posix_vports(names)
        return

    if sys.platform.startswith("win"):

        if args.win_bridge_ports:
            bridge = [p.strip() for p in args.win_bridge_ports.split(",")
                      if p.strip()]
            if args.vsp_tool == "hhd":
                bridge = ensure_hhd_pairs(args.channels, args.port, bridge)
        elif args.no_auto_com0com:
            ap.error("On Windows pass --win-bridge-ports, or drop "
                     "--no-auto-com0com to let the script set up com0com.")
        elif args.vsp_tool == "eltima":
            bridge = ensure_eltima_pairs(args.channels, args.port)
        elif args.vsp_tool == "hhd":
            bridge = ensure_hhd_pairs(args.channels, args.port, None)
        else:
            # Auto: install com0com if needed and create the app-side pairs
            # named <real_port>CHn; use the discovered bridge-side ports here.
            bridge = ensure_com0com_pairs(args.channels, args.port)

        WindowsVPorts(args.port, args.baud, args.channels, bridge,
                      args.max_payload, args.debug).run()

    else:
        if args.vport_names:
            names = [p.strip() for p in args.vport_names.split(",") if p.strip()]
            if len(names) != args.channels:
                ap.error(f"--vport-names needs {args.channels} names")
        else:
            names = default_posix_names(args.port, args.channels, args.link_prefix)
        PosixVPorts(args.port, args.baud, args.channels, names,
                    args.max_payload, args.debug).run()


if __name__ == "__main__":
    # Log the exact interpreter and argv at startup so a background run leaves
    # a trace even before any port is touched.
    _logline("=" * 60)
    _logline(f"startup: {sys.executable} {' '.join(sys.argv)}")
    try:
        main()
    except SystemExit as exc:
        # argparse / our own guided SystemExit messages: record and re-raise.
        if exc.code not in (0, None):
            _logline(f"exit: {exc}")
        raise
    except BaseException:  # noqa: BLE001
        import traceback
        tb = traceback.format_exc()
        # A startup exception here (most commonly serial.Serial() failing to
        # open the real port BEFORE any PTY/thread exists) is exactly what
        # makes a background run show no channel lines and no [PC->MCU] log.
        _dbg("FATAL startup/runtime exception:\n" + tb)
        raise



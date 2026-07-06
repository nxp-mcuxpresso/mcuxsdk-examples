#!/usr/bin/env python3
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
# Unified host-side parser for ALL debug data the wireless MCU can emit on the
# shared application console. A single capture (or a live serial session) may
# interleave three completely different data streams plus ordinary application
# logs; this script auto-detects and dispatches each line to the correct
# decoder, so the developer never has to pre-split the log:
#
#   1. NBU HCI logging          lines prefixed with '@'
#        '@<base64(direction(1) || packet_type(1) || payload)>'
#      The radio core (NBU) reports faults/asserts as HCI vendor events. These
#      lines are converted to a BTSNOOP file and then analysed by
#      debug_struct_parser.py (the NBU never emits a Zephyr coredump).
#
#   2. App-core (Cortex-M33) Zephyr coredump   block framed with '#CD:'
#        '#CD:BEGIN#' / '#CD:<base64>' ... / '#CD:END#'
#      Decoded to a binary Zephyr coredump consumable by the vendored
#      coredump_parser/log_parser.py and coredump_gdbserver.py.
#
#   3. Plain application console text   everything else
#      Banners, PRINTF output, shell prompts, etc. Tolerated/ignored (optionally
#      echoed) so a raw full-session capture can be fed in directly.
#
# This script does NOT re-implement the decoders: it imports the existing
# sibling modules
#     debug_struct_parser.py          (NBU struct analysis from BTSNOOP)
#     board_debug_coredump_decode.py  (#CD: framed Base64 -> binary coredump)
# and the vendored coredump_parser/log_parser.py for a coredump summary.
#
# Usage:
#   # Offline: parse a previously captured full application log.
#   python board_debug_parse.py -i capture.log
#
#   # Offline with named outputs + NBU BLE extension + ELF symbol resolution.
#   python board_debug_parse.py -i capture.log \
#       --btsnoop nbu.btsnoop --coredump coredump.bin \
#       --extension ble --elf build_vero/frdmkw43/lp_refdes_freertos/<app>.elf
#
#   # Live: open the COM port, capture until Ctrl+C, then parse everything.
#   python board_debug_parse.py -p /dev/ttyACM0 -b 115200 --save-log capture.log
#
#   # After a coredump is produced, launch the offline GDB server (needs ELF).
#   python board_debug_parse.py -i capture.log --coredump coredump.bin \
#       --elf <app>.elf --gdb

import argparse
import importlib.util
import os
import struct
import sys
import time

_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Line routing prefixes. Both are single ASCII tokens outside the Base64
# alphabet so they can never be confused with payload or with each other.
NBU_LINE_PREFIX = "@"
COREDUMP_PREFIX_STR = "#CD:"

# BTSNOOP constants (kept in sync with hci_to_btsnoop.py / debug_struct_parser.py).
BTSNOOP_MAGIC = b"btsnoop\x00"
BTSNOOP_VERSION = 1
BTSNOOP_DATALINK_HCI_UART = 1002
PACKET_FLAG_SENT = 0
PACKET_FLAG_RECEIVED = 1

# BLE HCI packet types.
HCI_CMD = 0x01
HCI_ACL = 0x02
HCI_EVENT = 0x04
HCI_ISO = 0x05
VALID_BLE_PACKET_TYPES = [HCI_CMD, HCI_ACL, HCI_EVENT, HCI_ISO]

# BTSNOOP timestamps are microseconds since 0000-01-01; this is the offset from
# the Unix epoch (same constant used by hci_to_btsnoop.py).
BTSNOOP_EPOCH_DELTA = 0x00DCDDB30F2F8000


def _load_sibling_module(module_name):
    """Import a module that lives next to this script by file path.

    Avoids depending on the caller's CWD / PYTHONPATH and avoids importing
    hci_to_btsnoop.py (which imports pyserial at module top) for the offline
    path - the BTSNOOP writer here is self-contained instead.
    """
    path = os.path.join(_SCRIPT_DIR, module_name + ".py")
    if not os.path.isfile(path):
        return None
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        return None
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


def _find_coredump_scripts_dir():
    """Locate components/debug/coredump/scripts relative to this file.

    The debug/ folder sits at
      mcuxsdk/examples/_common/project_segments/wireless/wireless_mcu/debug
    and the coredump host scripts at
      mcuxsdk/components/debug/coredump/scripts
    Walk up to the mcuxsdk root then descend; fall back to None if not found.
    """
    cur = _SCRIPT_DIR
    for _ in range(10):
        candidate = os.path.join(
            cur, "components", "debug", "coredump", "scripts"
        )
        if os.path.isdir(candidate):
            return candidate
        parent = os.path.dirname(cur)
        if parent == cur:
            break
        cur = parent
    return None


class BtsnoopWriter:
    """Minimal BTSNOOP (datalink 1002) writer.

    Produces a file byte-compatible with the one hci_to_btsnoop.py writes, so
    debug_struct_parser.extract_debug_from_btsnoop() consumes it unchanged.
    """

    def __init__(self, path):
        self.path = path
        self.fd = None
        self.packet_count = 0

    def __enter__(self):
        self.fd = open(self.path, "wb")
        self.fd.write(BTSNOOP_MAGIC)
        self.fd.write(struct.pack(">I", BTSNOOP_VERSION))
        self.fd.write(struct.pack(">I", BTSNOOP_DATALINK_HCI_UART))
        self.fd.flush()
        return self

    def __exit__(self, exc_type, exc, tb):
        if self.fd:
            self.fd.close()
        return False

    def write_packet(self, direction, packet_type, packet_data):
        full_packet = bytes([packet_type]) + packet_data
        orig_len = len(full_packet)
        flags = PACKET_FLAG_RECEIVED if direction == 0x01 else PACKET_FLAG_SENT
        timestamp = int(time.time() * 1000000) + BTSNOOP_EPOCH_DELTA
        self.fd.write(struct.pack(">I", orig_len))   # orig_len
        self.fd.write(struct.pack(">I", orig_len))   # incl_len
        self.fd.write(struct.pack(">I", flags))
        self.fd.write(struct.pack(">I", 0))          # drops
        self.fd.write(struct.pack(">Q", timestamp))
        self.fd.write(full_packet)
        self.fd.flush()
        self.packet_count += 1


def decode_nbu_line(line):
    """Decode one '@<base64>' NBU HCI line into (direction, packet_type, data).

    Returns None for any line that is not a valid NBU HCI packet line. Mirrors
    HCILogger.decode_base64_line() but is dependency-free (no pyserial import).
    """
    import base64

    line = line.strip()
    if not line.startswith(NBU_LINE_PREFIX):
        return None

    b64 = line[len(NBU_LINE_PREFIX):]
    if not b64:
        return None
    try:
        decoded = base64.b64decode(b64, validate=True)
    except Exception:
        return None

    if len(decoded) < 2:
        return None

    direction = decoded[0]
    if direction not in (0x00, 0x01):
        return None

    packet_type = decoded[1]
    if packet_type not in VALID_BLE_PACKET_TYPES:
        return None

    return (direction, packet_type, decoded[2:])


def _iter_lines(raw_text):
    """Yield logical lines, normalising CR / LF / CRLF endings."""
    normalized = raw_text.replace("\r\n", "\n").replace("\r", "\n")
    for line in normalized.split("\n"):
        yield line


def classify(raw_text, echo_plain=False):
    """One pass over the capture; returns counts of each stream that is present.

    This is only used for reporting / auto-detection. The heavy decoding is done
    by the dedicated decoders, which each re-scan and ignore foreign lines.
    """
    nbu_lines = 0
    cd_data_lines = 0
    saw_cd_begin = False
    saw_cd_end = False
    plain_lines = 0

    for line in _iter_lines(raw_text):
        stripped = line.strip()
        if not stripped:
            continue
        if stripped.startswith(NBU_LINE_PREFIX):
            nbu_lines += 1
        elif stripped == COREDUMP_PREFIX_STR + "BEGIN#":
            saw_cd_begin = True
        elif stripped == COREDUMP_PREFIX_STR + "END#":
            saw_cd_end = True
        elif stripped.startswith(COREDUMP_PREFIX_STR):
            cd_data_lines += 1
        else:
            plain_lines += 1
            if echo_plain:
                print(f"   [device] {stripped}")

    return {
        "nbu_lines": nbu_lines,
        "cd_data_lines": cd_data_lines,
        "cd_begin": saw_cd_begin,
        "cd_end": saw_cd_end,
        "plain_lines": plain_lines,
    }


def process_nbu(raw_text, btsnoop_path, extensions, elf):
    """Convert '@' NBU HCI lines to BTSNOOP, then analyse the debug struct."""
    dsp = _load_sibling_module("debug_struct_parser")
    if dsp is None:
        print("WARNING: debug_struct_parser.py not found; skipping NBU analysis.")
        return False

    print(f"\n{'='*70}")
    print("NBU HCI stream detected -> building BTSNOOP and analysing")
    print(f"{'='*70}")

    with BtsnoopWriter(btsnoop_path) as writer:
        for line_no, line in enumerate(_iter_lines(raw_text), start=1):
            stripped = line.strip()
            if not stripped.startswith(NBU_LINE_PREFIX):
                continue
            packet = decode_nbu_line(stripped)
            if packet is None:
                print(f"   (corrupt/unrecognised NBU line {line_no}, skipped)")
                continue
            direction, packet_type, data = packet
            writer.write_packet(direction, packet_type, data)
        n = writer.packet_count

    print(f"Wrote {n} HCI packet(s) to {btsnoop_path}")
    if n == 0:
        print("No valid NBU HCI packets decoded; nothing to analyse.")
        return False

    # Reuse the existing NBU debug-struct analysis end-to-end.
    extension_decoders = dsp.load_extension_decoders(extensions or [])
    try:
        data_list, stall_detected = dsp.extract_debug_from_btsnoop(btsnoop_path)
    except Exception as exc:
        print(f"WARNING: could not extract debug structures: {exc}")
        return False

    if not data_list:
        print("No NBU debug structure found in the HCI stream "
              "(NBU may not have faulted).")
    else:
        for i, data in enumerate(data_list, 1):
            if not data:
                continue
            if len(data_list) > 1:
                print(f"\n{'='*70}")
                print(f"NBU Debug Structure #{i} of {len(data_list)}")
                print(f"{'='*70}")
            try:
                debug_struct = dsp.NbuDebugStruct(data)
                dsp.analyze_debug_struct(debug_struct, extension_decoders, elf)
            except Exception as exc:
                print(f"WARNING: failed to analyse NBU structure #{i}: {exc}")

    if stall_detected:
        print(f"\n{'='*70}")
        print("ERROR: NBU is stuck - possible stall or deadlock detected")
        print(f"{'='*70}")

    return True


def _summarise_coredump(coredump_path):
    """Parse the binary coredump with the vendored log_parser for a summary."""
    scripts_dir = _find_coredump_scripts_dir()
    if scripts_dir is None:
        print("NOTE: coredump_parser/log_parser.py not found; "
              "skipping coredump summary.")
        return

    log_parser_path = os.path.join(
        scripts_dir, "coredump_parser", "log_parser.py"
    )
    if not os.path.isfile(log_parser_path):
        print("NOTE: log_parser.py not found; skipping coredump summary.")
        return

    # log_parser.py only imports logging+struct, so it loads standalone.
    spec = importlib.util.spec_from_file_location(
        "coredump_log_parser", log_parser_path
    )
    module = importlib.util.module_from_spec(spec)
    try:
        spec.loader.exec_module(module)
    except Exception as exc:
        print(f"NOTE: could not load log_parser.py ({exc}); skipping summary.")
        return

    import logging
    logging.basicConfig(level=logging.INFO, format="   [coredump] %(message)s")

    try:
        clf = module.CoredumpLogFile(coredump_path)
        clf.open()
        ok = clf.parse()
        clf.close()
    except Exception as exc:
        print(f"WARNING: log_parser could not parse {coredump_path}: {exc}")
        return

    if not ok:
        print("WARNING: log_parser reported the coredump did not parse cleanly.")
        return

    hdr = clf.log_hdr
    print(f"   Coredump header: reason={module.reason_string(hdr['reason'])} "
          f"({hdr['reason']}), tgt_code={hdr['tgt_code']}, "
          f"ptr_size={hdr['ptr_size']}")
    regions = clf.get_memory_regions()
    print(f"   Memory regions: {len(regions)}")
    for r in regions:
        print(f"     0x{r['start']:08x}..0x{r['end']:08x} "
              f"({r['end'] - r['start']} bytes)")


def process_coredump(raw_text, coredump_path, require_markers, elf, do_gdb):
    """Decode the '#CD:' framed Base64 block to a binary Zephyr coredump."""
    bdcd = _load_sibling_module("board_debug_coredump_decode")
    if bdcd is None:
        print("WARNING: board_debug_coredump_decode.py not found; "
              "skipping coredump decode.")
        return False

    print(f"\n{'='*70}")
    print("App-core Zephyr coredump detected -> decoding")
    print(f"{'='*70}")

    try:
        data = bdcd.decode(raw_text, require_markers=require_markers)
    except ValueError as exc:
        print(f"WARNING: coredump decode failed: {exc}")
        return False

    with open(coredump_path, "wb") as f:
        f.write(data)

    magic = data[:2]
    magic_note = "OK" if magic == b"ZE" else "UNEXPECTED (expected b'ZE')"
    print(f"Wrote {len(data)} bytes to {coredump_path}; "
          f"header magic={magic!r} ({magic_note})")

    _summarise_coredump(coredump_path)

    scripts_dir = _find_coredump_scripts_dir()
    gdbserver = (
        os.path.join(scripts_dir, "coredump_gdbserver.py")
        if scripts_dir else None
    )
    if elf and gdbserver and os.path.isfile(gdbserver):
        cmd = (f"python {gdbserver} {elf} {coredump_path}")
        if do_gdb:
            print(f"\nLaunching coredump GDB server:\n  {cmd}")
            import subprocess
            subprocess.run([sys.executable, gdbserver, elf, coredump_path])
        else:
            print("\nFor offline GDB attach, run:")
            print(f"  {cmd}")
            print("  # then in another shell:")
            print(f"  arm-none-eabi-gdb {elf} -ex 'target remote :1234'")
    elif gdbserver and os.path.isfile(gdbserver):
        print("\nFor offline GDB attach, provide --elf and run:")
        print(f"  python {gdbserver} <app>.elf {coredump_path}")

    return True


def capture_serial(port, baudrate, save_log):
    """Open the serial port and capture text until Ctrl+C; return the text."""
    try:
        import serial
    except ImportError:
        print("ERROR: pyserial is required for live capture (-p). "
              "Install it (pip install pyserial) or use -i with a saved log.")
        sys.exit(2)

    print(f"Opening serial port {port} at {baudrate} baud "
          f"(Ctrl+C to stop and parse)...")
    ser = serial.Serial(
        port=port,
        baudrate=baudrate,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=1.0,
    )

    chunks = []
    tee = open(save_log, "w", encoding="utf-8") if save_log else None
    try:
        while True:
            data = ser.readline()
            if not data:
                continue
            text = data.decode("utf-8", errors="replace")
            sys.stdout.write(text)
            sys.stdout.flush()
            chunks.append(text)
            if tee:
                tee.write(text)
                tee.flush()
    except KeyboardInterrupt:
        print("\n\nCapture stopped by user; parsing captured data...")
    finally:
        if ser.is_open:
            ser.close()
        if tee:
            tee.close()
            print(f"Raw capture saved to {save_log}")

    return "".join(chunks)


def main(argv=None):
    parser = argparse.ArgumentParser(
        description=(
            "Unified parser for all wireless-MCU console debug data: NBU HCI "
            "logging ('@' lines), the app-core Zephyr coredump ('#CD:' block), "
            "and plain application text. Accepts a saved log file or a live "
            "serial port and is robust to arbitrarily interleaved app logs."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument(
        "-i", "--input",
        help="Parse a previously saved console/application log text file.",
    )
    source.add_argument(
        "-p", "--port",
        help="Capture live from a serial port (e.g. /dev/ttyACM0, COM3), "
             "then parse on Ctrl+C.",
    )
    parser.add_argument(
        "-b", "--baudrate", type=int, default=115200,
        help="Serial baudrate for -p (default: 115200).",
    )
    parser.add_argument(
        "--save-log",
        help="When capturing live (-p), also tee the raw text to this file.",
    )
    parser.add_argument(
        "--btsnoop", default=None,
        help="Output BTSNOOP path for the NBU HCI stream "
             "(default: <base>.btsnoop).",
    )
    parser.add_argument(
        "--coredump", default=None,
        help="Output binary Zephyr coredump path for the app-core stream "
             "(default: <base>.coredump.bin).",
    )
    parser.add_argument(
        "--extension", "-e", action="append", dest="extensions",
        help="NXP internal use only. NBU debug-struct extension decoder "
             "(repeatable). Available: ble",
    )
    parser.add_argument(
        "--elf", default=None,
        help="ELF for symbol resolution: passed to the NBU analyser and used "
             "for the offline coredump GDB-server command.",
    )
    parser.add_argument(
        "--no-markers", action="store_true",
        help="Decode every '#CD:' data line without requiring "
             "#CD:BEGIN#/#CD:END# (use for truncated coredump captures).",
    )
    parser.add_argument(
        "--gdb", action="store_true",
        help="After decoding a coredump, launch coredump_gdbserver.py "
             "(requires --elf).",
    )
    parser.add_argument(
        "--echo-plain", action="store_true",
        help="Echo plain (non-debug) application console lines while scanning.",
    )
    args = parser.parse_args(argv)

    # Acquire the capture text (offline file or live serial).
    if args.input:
        if not os.path.isfile(args.input):
            print(f"ERROR: input file not found: {args.input}")
            return 1
        with open(args.input, "r", encoding="utf-8", errors="replace") as f:
            raw_text = f.read()
        base = os.path.splitext(os.path.basename(args.input))[0]
    else:
        raw_text = capture_serial(args.port, args.baudrate, args.save_log)
        base = args.save_log and os.path.splitext(
            os.path.basename(args.save_log))[0] or "capture"

    if args.elf and not os.path.isfile(args.elf):
        print(f"WARNING: ELF '{args.elf}' not found; "
              "continuing without symbol resolution.")
        args.elf = None

    btsnoop_path = args.btsnoop or (base + ".btsnoop")
    coredump_path = args.coredump or (base + ".coredump.bin")

    # Auto-detect which streams are present and report.
    info = classify(raw_text, echo_plain=args.echo_plain)
    print(f"\n{'='*70}")
    print("Capture summary")
    print(f"{'='*70}")
    print(f"  NBU HCI lines      : {info['nbu_lines']}")
    print(f"  Coredump data lines: {info['cd_data_lines']} "
          f"(BEGIN={info['cd_begin']}, END={info['cd_end']})")
    print(f"  Plain text lines   : {info['plain_lines']}")

    produced_any = False

    if info["nbu_lines"] > 0:
        if process_nbu(raw_text, btsnoop_path, args.extensions, args.elf):
            produced_any = True

    if info["cd_data_lines"] > 0 or info["cd_begin"]:
        require_markers = not args.no_markers
        if process_coredump(raw_text, coredump_path, require_markers,
                            args.elf, args.gdb):
            produced_any = True

    if not produced_any:
        print(f"\n{'='*70}")
        print("No NBU HCI or app-core coredump data found in the capture.")
        print("Only plain application text was present (nothing to decode).")
        print(f"{'='*70}")
        return 0

    print(f"\n{'='*70}")
    print("Done.")
    print(f"{'='*70}")
    return 0


if __name__ == "__main__":
    sys.exit(main())

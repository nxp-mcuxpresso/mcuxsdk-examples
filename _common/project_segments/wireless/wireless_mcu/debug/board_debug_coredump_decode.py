#!/usr/bin/env python3
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
# Host decoder for the board_debug coredump-over-console transport.
#
# The on-target backend (board_debug_coredump.c) streams a Zephyr coredump as
# framed Base64 lines over the application console:
#
#     #CD:BEGIN#
#     #CD:<base64>
#     #CD:<base64>
#     ...
#     #CD:END#
#
# This script extracts that framed block from a raw console capture (the kind
# produced by .mcu-harness/scripts/flash_and_capture.py, a terminal log, or any
# text dump of the UART), strips the "#CD:" prefix from each data line,
# concatenates and Base64-decodes the payload, and writes the resulting binary
# Zephyr coredump. That binary is directly consumable by the upstream Zephyr
# coredump tooling already vendored in the SDK:
#
#     components/debug/coredump/scripts/coredump_parser/log_parser.py
#     components/debug/coredump/scripts/coredump_gdbserver.py
#
# Typical offline GDB attach workflow:
#
#     # 1. Decode the console capture to a binary coredump.
#     python board_debug_coredump_decode.py capture.log -o coredump.bin
#
#     # 2. Start the Zephyr coredump GDB server against the ELF + binary dump.
#     python <sdk>/mcuxsdk/components/debug/coredump/scripts/coredump_gdbserver.py \
#         build/<board>/<app>/<app>.elf coredump.bin
#
#     # 3. In another shell, attach GDB to read PC / LR / SP and the faulting frame.
#     arm-none-eabi-gdb build/<board>/<app>/<app>.elf \
#         -ex "target remote :1234"
#
# The script is transport-agnostic: it ignores any non-"#CD:" console text
# (banners, application logs, NBU HCI "@" lines), so a full session capture can
# be fed in directly. It also tolerates carriage returns, leading/trailing
# whitespace, and partial captures (the BEGIN/END markers are used only to
# bound the block when present; with --no-markers the markers are not required).

import argparse
import base64
import binascii
import sys

# Keep in sync with board_debug_coredump.c / upstream Zephyr coredump strings.
COREDUMP_PREFIX_STR = "#CD:"
COREDUMP_BEGIN_LINE = "#CD:BEGIN#"
COREDUMP_END_LINE = "#CD:END#"


def _iter_text_lines(raw_text):
    """Yield logical lines from a raw capture.

    Splits on both '\n' and '\r' so a capture using CR, LF, or CRLF line
    endings all decode the same way. Empty fragments are skipped by the
    caller after stripping.
    """
    # Normalize CRLF and lone CR to LF, then split.
    normalized = raw_text.replace("\r\n", "\n").replace("\r", "\n")
    for line in normalized.split("\n"):
        yield line


def extract_payload(raw_text, require_markers=True):
    """Return the concatenated Base64 payload string from a console capture.

    Walks the capture line by line. When require_markers is True, only the
    "#CD:" data lines strictly between "#CD:BEGIN#" and "#CD:END#" are
    collected (this is the normal, robust path). When require_markers is
    False, every "#CD:" data line is collected regardless of markers, which
    is useful for a truncated capture missing one of the markers.

    Raises ValueError when require_markers is True and a complete
    BEGIN..END block cannot be found.
    """
    payload_chunks = []
    in_block = not require_markers
    saw_begin = False
    saw_end = False

    for line in _iter_text_lines(raw_text):
        stripped = line.strip()
        if not stripped:
            continue

        if stripped == COREDUMP_BEGIN_LINE:
            in_block = True
            saw_begin = True
            # Reset any payload collected before a (re)start marker so a
            # capture containing multiple dumps yields the last complete one.
            payload_chunks = []
            continue

        if stripped == COREDUMP_END_LINE:
            if require_markers:
                saw_end = True
                # Stop at the first complete block end when markers required.
                in_block = False
                break
            # In no-markers mode an END is just a boundary; keep scanning.
            continue

        if not in_block:
            continue

        if not stripped.startswith(COREDUMP_PREFIX_STR):
            # Console noise interleaved inside the block (should not happen on
            # a healthy capture, but tolerate it): ignore non-prefixed lines.
            continue

        b64 = stripped[len(COREDUMP_PREFIX_STR):]
        if b64:
            payload_chunks.append(b64)

    if require_markers:
        if not saw_begin:
            raise ValueError(
                f"no {COREDUMP_BEGIN_LINE!r} marker found in capture; "
                "use --no-markers to decode all #CD: lines regardless"
            )
        if not saw_end:
            raise ValueError(
                f"found {COREDUMP_BEGIN_LINE!r} but no matching "
                f"{COREDUMP_END_LINE!r}; capture may be truncated "
                "(use --no-markers to decode what was captured)"
            )

    if not payload_chunks:
        raise ValueError("no #CD: coredump data lines found in capture")

    return "".join(payload_chunks)


def decode(raw_text, require_markers=True):
    """Decode a console capture to the raw binary Zephyr coredump bytes."""
    b64 = extract_payload(raw_text, require_markers=require_markers)
    try:
        # validate=True rejects stray non-Base64 characters so a corrupted
        # capture fails loudly instead of silently producing garbage.
        data = base64.b64decode(b64, validate=True)
    except binascii.Error as exc:
        raise ValueError(f"Base64 decode failed: {exc}") from exc
    return data


def main(argv=None):
    parser = argparse.ArgumentParser(
        description=(
            "Decode a board_debug coredump-over-console capture (#CD: framed "
            "Base64) into a binary Zephyr coredump consumable by "
            "coredump_parser/log_parser.py and coredump_gdbserver.py."
        )
    )
    parser.add_argument(
        "input",
        nargs="?",
        default="-",
        help="Console capture file (default: stdin '-').",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="-",
        help="Output binary coredump file (default: stdout '-').",
    )
    parser.add_argument(
        "--no-markers",
        action="store_true",
        help=(
            "Do not require #CD:BEGIN#/#CD:END# markers; decode every #CD: "
            "data line found. Use for truncated captures."
        ),
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Print decode summary (byte count, magic check) to stderr.",
    )
    args = parser.parse_args(argv)

    if args.input == "-":
        raw_text = sys.stdin.read()
    else:
        with open(args.input, "r", encoding="utf-8", errors="replace") as f:
            raw_text = f.read()

    try:
        data = decode(raw_text, require_markers=not args.no_markers)
    except ValueError as exc:
        sys.stderr.write(f"ERROR: {exc}\n")
        return 1

    if args.verbose:
        magic = data[:2]
        magic_ok = magic == b"ZE"
        magic_note = "OK" if magic_ok else "UNEXPECTED, expected b'ZE'"
        sys.stderr.write(
            f"[decode] {len(data)} bytes decoded; "
            f"header magic={magic!r} ({magic_note})\n"
        )
        if not magic_ok:
            sys.stderr.write(
                "[decode] WARNING: first two bytes are not the Zephyr "
                "coredump magic 'ZE'; the capture may be misaligned.\n"
            )

    if args.output == "-":
        sys.stdout.buffer.write(data)
        sys.stdout.buffer.flush()
    else:
        with open(args.output, "wb") as f:
            f.write(data)
        if args.verbose:
            sys.stderr.write(f"[decode] wrote {args.output}\n")

    return 0


if __name__ == "__main__":
    sys.exit(main())

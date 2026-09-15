#!/usr/bin/env python3
"""
test_loopback.py - Self-test the COBS framing without any real hardware.

It verifies that build_frame + the receiver de-framing round-trips
per-channel data correctly, including payloads that contain 0x00 bytes
(the whole reason COBS is used).

Run:
    python3 test_loopback.py
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from serial_mux_hub import cobs_encode, cobs_decode, build_frame


def deframe_stream(stream: bytes):
    """Yield (channel, payload) tuples from a raw framed byte stream."""
    buf = bytearray(stream)
    while True:
        try:
            zero = buf.index(0)
        except ValueError:
            break
        body = bytes(buf[:zero])
        del buf[:zero + 1]
        if not body:
            continue
        try:
            decoded = cobs_decode(body)
        except ValueError:
            continue
        if decoded:
            yield decoded[0], decoded[1:]


def main():
    cases = [
        (0, b"hello world\n"),
        (1, b"\x00\x00\x00binary zeros\x00"),
        (2, bytes(range(256))),            # every byte value incl 0x00 and 0xFF
        (3, b"A" * 300),                   # longer than one COBS run
        (0, b""),                          # empty payload (should be skipped by hub)
    ]

    # 1) COBS bijection check
    for _, payload in cases:
        assert cobs_decode(cobs_encode(payload)) == payload, "COBS round-trip failed"
    print("PASS: COBS encode/decode round-trips all payloads")

    # 2) Framing + de-framing, interleaved on one wire
    stream = b"".join(build_frame(ch, p) for ch, p in cases if p)
    got = list(deframe_stream(stream))
    expect = [(ch, p) for ch, p in cases if p]
    assert got == expect, f"de-frame mismatch:\n got={got}\n exp={expect}"
    print("PASS: interleaved multi-channel framing round-trips")

    # 3) Corruption / resync: inject garbage between frames
    noisy = build_frame(1, b"good1") + b"\x05\x05" + b"\x00" + build_frame(2, b"good2")
    got = list(deframe_stream(noisy))
    assert (1, b"good1") in got and (2, b"good2") in got, "resync failed"
    print("PASS: receiver resynchronizes after a corrupt frame")

    print("\nAll tests passed.")


if __name__ == "__main__":
    main()

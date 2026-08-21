# pytest unit tests for the run.py frame codec and INFO/RESULT parsers.
#
# Contract source (board firmware, the ground truth):
#   - examples/eiq_examples/executorch_modelrunner/protocol.h
#   - examples/eiq_examples/executorch_modelrunner/serial_protocol.cpp  (send_frame / recv_frame)
#   - examples/eiq_examples/executorch_modelrunner/protocol.cpp         (crc16_ccitt)
#   - examples/eiq_examples/executorch_modelrunner/executorch_engine.cpp (INFO/RESULT layouts)
#
# Frame: [0x55][0xAA][CMD][LEN:4B LE][PAYLOAD:LEN][CRC16:2B LE]
# CRC-16/CCITT-FALSE over CMD+LEN+PAYLOAD (init=0xFFFF, poly=0x1021).
import struct

import pytest

from run import (
    DTYPE_NAMES,
    build_frame,
    crc16_ccitt,
    parse_frame,
    parse_info,
    parse_result,
)


def test_crc_check_vector():
    # CRC-16/CCITT-FALSE of ASCII "123456789" == 0x29B1 (standard check vector).
    # This matches protocol.cpp's documented check value.
    assert crc16_ccitt(b"123456789") == 0x29B1


def test_crc_known_frame_prefix():
    # Mirror the board's send_frame range exactly: CRC over CMD+LEN(4)+PAYLOAD,
    # i.e. SKIP the two SYNC bytes. Recompute by hand and compare to build_frame.
    payload = b"hello"
    cmd = 0x81
    crc_input = bytes([cmd]) + struct.pack("<I", len(payload)) + payload
    expected = crc16_ccitt(crc_input)
    frame = build_frame(cmd=cmd, payload=payload)
    sent_crc = struct.unpack("<H", frame[-2:])[0]
    assert sent_crc == expected


def test_frame_layout_sync_len_crc():
    frame = build_frame(cmd=0x01, payload=b"\x01\x02\x03")
    assert frame[0:2] == b"\x55\xAA"
    assert frame[2] == 0x01
    assert struct.unpack("<I", frame[3:7])[0] == 3
    assert frame[7:10] == b"\x01\x02\x03"
    # trailing 2 bytes are CRC LE
    assert len(frame) == 2 + 1 + 4 + 3 + 2


def test_frame_roundtrip_binary_safe():
    # payload contains bytes that would break text protocols: 0x0A 0x0D 0x55 0xAA 0x00
    payload = bytes([0x55, 0xAA, 0x0A, 0x0D, 0x00, 0xFF])
    frame = build_frame(cmd=0x01, payload=payload)
    cmd, pl = parse_frame(frame)
    assert cmd == 0x01
    assert pl == payload


def test_parse_frame_rejects_bad_sync():
    bad = bytes([0x00, 0x00, 0x01]) + struct.pack("<I", 0)
    with pytest.raises(ValueError):
        parse_frame(bad)


def test_parse_frame_detects_crc_corruption():
    frame = bytearray(build_frame(cmd=0x01, payload=b"abc"))
    frame[-1] ^= 0xFF  # flip a CRC bit
    with pytest.raises(ValueError):
        parse_frame(bytes(frame))


def test_parse_info():
    # INFO entry layout:
    #   dtype:u8  ndim:u8  sizes:ndim*i32LE  dim_order:ndim*u8  nbytes:u32LE
    # NOTE: dtype byte == raw ExecuTorch ScalarType enum value. Per
    #   runtime/core/portable_type/scalar_type.h: Byte=0, Char=1, Short=2,
    #   Int=3, Long=4, Half=5, Float=6. So dtype byte 1 here is Char (int8),
    #   NOT Float. The board writes static_cast<uint8_t>(scalar_type).
    sizes = [1, 3, 32, 32]
    info = bytes([1,                # n_inputs
                  1, 4])            # input0: dtype=1(Char), ndim=4
    for s in sizes:
        info += s.to_bytes(4, "little")
    info += bytes([0, 2, 3, 1])     # dim_order
    info += (12288).to_bytes(4, "little")
    info += bytes([1])              # n_outputs=1 (body truncated; we only parse inputs)

    parsed = parse_info(info)
    assert parsed["n_inputs"] == 1
    assert parsed["inputs"][0]["dtype"] == 1
    assert parsed["inputs"][0]["shape"] == [1, 3, 32, 32]
    assert parsed["inputs"][0]["dim_order"] == [0, 2, 3, 1]
    assert parsed["inputs"][0]["nbytes"] == 12288
    assert parsed["n_outputs"] == 1
    assert parsed["outputs"] == []


def test_parse_info_float_dtype():
    # dtype byte 6 == Float per ScalarType enum. Verify the parser leaves dtype
    # as the raw int and DTYPE_NAMES maps it.
    info = bytes([1, 6, 0])  # 1 input, dtype=6(Float), ndim=0 (scalar)
    info += (4).to_bytes(4, "little")  # nbytes for a float32 scalar
    info += bytes([0])  # n_outputs
    parsed = parse_info(info)
    assert parsed["inputs"][0]["dtype"] == 6
    assert DTYPE_NAMES[6] == "Float"


def test_parse_result_cifarnet_like():
    # RESULT: timing_us:u64LE  n_outputs:u8
    #   per output: dtype:u8  ndim:u8  sizes:ndim*i32LE  nbytes:u32LE  data:nbytes
    # NOTE: RESULT output entries have NO dim_order field (only INFO does).
    timing = 123456
    # one output: shape [1,10], dtype Char(1), 10 int8 bytes
    out_shape = [1, 10]
    out_data = bytes(range(10))
    payload = struct.pack("<Q", timing)
    payload += bytes([1,           # n_outputs
                      1, 2])       # output0: dtype=Char(1), ndim=2
    for s in out_shape:
        payload += struct.pack("<i", s)
    payload += struct.pack("<I", len(out_data))
    payload += out_data

    parsed = parse_result(payload)
    assert parsed["timing_us"] == timing
    assert parsed["n_outputs"] == 1
    o = parsed["outputs"][0]
    assert o["dtype"] == 1
    assert o["shape"] == [1, 10]
    assert o["nbytes"] == 10
    assert o["data"] == out_data


def test_parse_result_multiple_outputs():
    payload = struct.pack("<Q", 42)
    payload += bytes([2])  # n_outputs
    # output0: Float(6), shape [1], 4 bytes
    payload += bytes([6, 1]) + struct.pack("<i", 1) + struct.pack("<I", 4) + b"\x00\x00\x80\x3f"
    # output1: Char(1), shape [], 1 byte (scalar)
    payload += bytes([1, 0]) + struct.pack("<I", 1) + b"\x07"
    parsed = parse_result(payload)
    assert parsed["timing_us"] == 42
    assert parsed["n_outputs"] == 2
    assert parsed["outputs"][0]["shape"] == [1]
    assert parsed["outputs"][1]["shape"] == []
    assert parsed["outputs"][1]["data"] == b"\x07"

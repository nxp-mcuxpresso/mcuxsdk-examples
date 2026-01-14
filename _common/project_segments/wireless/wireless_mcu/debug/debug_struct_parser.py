'''
* Copyright 2025-2026 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
'''

#!/usr/bin/env python3
"""
NBU Debug Structure Parser

This script parses the NBU debug structure from a binary file, hex dump, or BTSNOOP file
and provides essential debug information.

Usage:
    python debug_struct_parser.py <input_file> [--format bin|hex|btsnoop]

Examples:
    # Parse from binary file
    python debug_struct_parser.py debug_struct.bin

    # Parse from hex dump
    python debug_struct_parser.py debug_dump.txt --format hex

    # Parse from BTSNOOP capture
    python debug_struct_parser.py hci_capture.btsnoop --format btsnoop
"""

import struct
import sys
import argparse

# Constants from fwk_debug_struct.h
NBUDBG_VERSION = 2
NBUDBG_BLE_STRUCT_SIZE = 0x100
NBUDBG_ASSERT_FILE_NAME_SIZE = 74
NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC = 0x00A55E27
NBUDBG_HANDLER_MODE_MAGIC = 0xCAFE0000
NBUDBG_MODE_MAGIC_MASK = 0xFFFF0000
NBUDBG_IRQ_NUMBER_MASK = 0x0000FFFF
NBUDBG_MAX_NB_WARNINGS = 7

# BTSNOOP constants
BTSNOOP_MAGIC = b'btsnoop\x00'
BTSNOOP_DATALINK_HCI_UART = 1002

# HCI packet types
HCI_EVENT = 0x04
HCI_VENDOR_EVENT = 0xFF
DEBUG_SUBEVENT = 0xF1
DEBUG_STRUCT_BUFFER_ID = 0
STALL_BUFFER_ID = 2

class RegInfo:
    """Processor register information - 76 bytes total"""
    def __init__(self, data: bytes, offset: int = 0):
        # 19 uint32_t values = 76 bytes
        fmt = '<19I'
        values = struct.unpack_from(fmt, data, offset)

        self.cfsr = values[0]
        self.xfar = values[1]
        self.pc = values[2]
        self.lr = values[3]
        self.sp = values[4]
        self.psr = values[5]
        self.r0 = values[6]
        self.r1 = values[7]
        self.r2 = values[8]
        self.r3 = values[9]
        self.r4 = values[10]
        self.r5 = values[11]
        self.r6 = values[12]
        self.r7 = values[13]
        self.r8 = values[14]
        self.r9 = values[15]
        self.r10 = values[16]
        self.r11 = values[17]
        self.r12 = values[18]


class AssertInfo:
    """Assert information - 76 bytes total"""
    def __init__(self, data: bytes, offset: int = 0):
        # uint16_t line (2 bytes) + char file_name[74] (74 bytes) = 76 bytes
        self.line = struct.unpack_from('<H', data, offset)[0]
        # File name starts immediately after line field (no padding)
        file_name_bytes = data[offset + 2:offset + 2 + NBUDBG_ASSERT_FILE_NAME_SIZE]
        null_pos = file_name_bytes.find(b'\x00')
        if null_pos != -1:
            file_name_bytes = file_name_bytes[:null_pos]
        self.file_name = file_name_bytes.decode('utf-8', errors='replace')


class ExecutionContext:
    """Execution context information - 12 bytes total (union)"""
    def __init__(self, data: bytes, offset: int = 0):
        # Union is 12 bytes (max of uint32_t or struct with uint32_t + char[8])
        self.handler_irq = struct.unpack_from('<I', data, offset)[0]

        if (self.handler_irq & NBUDBG_MODE_MAGIC_MASK) == NBUDBG_HANDLER_MODE_MAGIC:
            self.is_handler_mode = True
            self.irq_number = self.handler_irq & NBUDBG_IRQ_NUMBER_MASK
            self.thread_addr = None
            self.thread_name = None
        else:
            self.is_handler_mode = False
            self.irq_number = None
            self.thread_addr = self.handler_irq
            # Thread name is at offset + 4 (after thread_addr uint32_t)
            thread_name_bytes = data[offset + 4:offset + 12]
            null_pos = thread_name_bytes.find(b'\x00')
            if null_pos != -1:
                thread_name_bytes = thread_name_bytes[:null_pos]
            self.thread_name = thread_name_bytes.decode('utf-8', errors='replace')


class NbuDebugInfo:
    """NBU debug information structure - 104 bytes total"""
    def __init__(self, data: bytes, offset: int = 0):
        # exception_id at offset + 0 (4 bytes)
        self.exception_id = struct.unpack_from('<I', data, offset)[0]
        # nbu_sha1 at offset + 4 (4 bytes)
        self.nbu_sha1 = struct.unpack_from('<I', data, offset + 4)[0]

        # Union starts at offset + 8 (76 bytes)
        if self.exception_id == NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC:
            self.is_assert = True
            self.assert_info = AssertInfo(data, offset + 8)
            self.reg_info = None
        else:
            self.is_assert = False
            self.reg_info = RegInfo(data, offset + 8)
            self.assert_info = None

        # execution_context starts at offset + 8 + 76 = offset + 84 (12 bytes)
        self.execution_context = ExecutionContext(data, offset + 84)

        # warnings array starts at offset + 96 (7 bytes)
        self.warnings = list(struct.unpack_from('<7B', data, offset + 96))

        # warning_index at offset + 103 (1 byte)
        self.warning_index = struct.unpack_from('<B', data, offset + 103)[0]

class NbuDebugStruct:
    """Complete NBU debug structure"""
    def __init__(self, data: bytes):
        if len(data) < 16:
            raise ValueError("Input data too small to contain debug structure header")

        # Header: version (2) + logging_buf_size (2) + logging_buf_offset (4) = 8 bytes
        self.version = struct.unpack_from('<H', data, 0)[0]
        self.logging_buf_size = struct.unpack_from('<H', data, 2)[0]
        self.logging_buf_offset = struct.unpack_from('<I', data, 4)[0]

        # nbu_dbg_info starts at offset 8 (104 bytes)
        self.nbu_dbg_info = NbuDebugInfo(data, 8)

        # BLE debug data starts at offset 8 + 104 = 112 (was 104)
        ble_offset = 112
        if len(data) >= ble_offset + NBUDBG_BLE_STRUCT_SIZE:
            self.dbg_ble = data[ble_offset:ble_offset + NBUDBG_BLE_STRUCT_SIZE]
        else:
            self.dbg_ble = b''


def print_raw_data(label: str, data: bytes):
    """Print raw data in hex dump format"""
    print(f"\n{label} ({len(data)} bytes):")
    for i in range(0, len(data), 16):
        hex_str = ' '.join(f'{b:02X}' for b in data[i:i+16])
        print(f"  {i:04X}: {hex_str}")


def analyze_debug_struct(debug_struct: NbuDebugStruct):
    """Analyze and print debug structure information"""
    print(f"\nNBU Debug Structure Analysis")
    print("=" * 60)
    print(f"Version: 0x{debug_struct.version:04X}")

    if debug_struct.version != NBUDBG_VERSION:
        print(f"WARNING: Expected version 0x{NBUDBG_VERSION:04X}, got 0x{debug_struct.version:04X}")

    nbu_info = debug_struct.nbu_dbg_info

    print(f"NBU SHA1: 0x{nbu_info.nbu_sha1:08X}")

    # Display warnings if any
    if any(w != 0 for w in nbu_info.warnings):
        print("\n*** WARNING INFORMATION ***")
        print(f"Warning Index: {nbu_info.warning_index}")
        print("Warning Circular Buffer:")
        for i, warning_id in enumerate(nbu_info.warnings):
            marker = "->" if i == nbu_info.warning_index else "  "
            print(f"{marker}{i}: {warning_id}")

    # Check if there's an assert or fault
    has_assert = nbu_info.exception_id == NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC
    has_fault = (nbu_info.exception_id != NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC and
                 nbu_info.exception_id != 0)

    if has_assert:
        print("\n*** NBU ASSERT DETECTED ***")
        print(f"  Exception ID: 0x{nbu_info.exception_id:08X}")
        print(f"  File: {nbu_info.assert_info.file_name}")
        print(f"  Line: {nbu_info.assert_info.line}")
    elif has_fault:
        print("\n*** NBU FAULT DETECTED ***")
        print(f"  Exception ID: 0x{nbu_info.exception_id:08X}")

        regs = nbu_info.reg_info
        print("\nKey Registers:")
        print(f"  PC  (Program Counter): 0x{regs.pc:08X}")
        print(f"  LR  (Link Register):   0x{regs.lr:08X}")
        print(f"  SP  (Stack Pointer):   0x{regs.sp:08X}")
        print(f"  CFSR (Fault Status):   0x{regs.cfsr:08X}")
        print(f"  xFAR (Fault Address):  0x{regs.xfar:08X}")

        print("\nAll Registers:")
        print(f"  R0:  0x{regs.r0:08X}  R1:  0x{regs.r1:08X}  R2:  0x{regs.r2:08X}  R3:  0x{regs.r3:08X}")
        print(f"  R4:  0x{regs.r4:08X}  R5:  0x{regs.r5:08X}  R6:  0x{regs.r6:08X}  R7:  0x{regs.r7:08X}")
        print(f"  R8:  0x{regs.r8:08X}  R9:  0x{regs.r9:08X}  R10: 0x{regs.r10:08X}  R11: 0x{regs.r11:08X}")
        print(f"  R12: 0x{regs.r12:08X}  PSR: 0x{regs.psr:08X}")

    # Only display execution context if there's an assert or fault
    if has_assert or has_fault:
        print("\nExecution Context:")
        ctx = nbu_info.execution_context
        if ctx.is_handler_mode:
            print(f"  Mode: Handler (IRQ {ctx.irq_number})")
        else:
            print(f"  Mode: Thread")
            print(f"  Thread Address: 0x{ctx.thread_addr:08X}")
            print(f"  Thread Name:    {ctx.thread_name}")

    if debug_struct.dbg_ble:
        print_raw_data("\nBLE Debug Data", debug_struct.dbg_ble)

    print("\n" + "=" * 60)


def read_binary_file(filename: str) -> bytes:
    """Read binary file"""
    with open(filename, 'rb') as f:
        return f.read()


def read_hex_file(filename: str) -> bytes:
    """Read hex dump file and convert to binary"""
    data = bytearray()
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue

            for word in line.split():
                word = word.replace('0x', '').replace(':', '').replace(',', '')
                try:
                    if len(word) == 2:
                        data.append(int(word, 16))
                    elif len(word) > 2 and len(word) % 2 == 0:
                        for i in range(0, len(word), 2):
                            data.append(int(word[i:i+2], 16))
                except ValueError:
                    continue

    return bytes(data)

def extract_debug_from_btsnoop(filename: str) -> tuple:
    """Extract debug structures from BTSNOOP file"""
    debug_structures = []
    debug_data = bytearray()
    segment_count = 0
    stall_detected = False

    with open(filename, 'rb') as f:
        # Read and verify header
        magic = f.read(8)
        if magic != BTSNOOP_MAGIC:
            raise ValueError("Not a valid BTSNOOP file")

        version = struct.unpack('>I', f.read(4))[0]
        datalink = struct.unpack('>I', f.read(4))[0]

        if datalink != BTSNOOP_DATALINK_HCI_UART:
            raise ValueError(f"Unsupported datalink type: {datalink}")

        # Read packets
        while True:
            # Read packet record header (24 bytes)
            header = f.read(24)
            if len(header) < 24:
                break

            orig_len = struct.unpack('>I', header[0:4])[0]
            incl_len = struct.unpack('>I', header[4:8])[0]
            flags = struct.unpack('>I', header[8:12])[0]
            drops = struct.unpack('>I', header[12:16])[0]
            timestamp = struct.unpack('>Q', header[16:24])[0]

            # Read packet data
            packet_data = f.read(incl_len)
            if len(packet_data) < incl_len:
                break

            # Check if this is an HCI event packet
            if len(packet_data) < 1:
                continue

            packet_type = packet_data[0]
            if packet_type != HCI_EVENT:
                continue

            # Parse HCI event: [type][event_code][param_len][params...]
            if len(packet_data) < 3:
                continue

            event_code = packet_data[1]
            param_len = packet_data[2]

            if event_code != HCI_VENDOR_EVENT:
                continue

            if len(packet_data) < 3 + param_len:
                continue

            params = packet_data[3:3 + param_len]

            if len(params) < 3:
                continue

            subevent = params[0]
            buffer_id = params[1]
            last_segment = params[2]
            data = params[3:]

            # Check for NBU stall indication
            if subevent == DEBUG_SUBEVENT and buffer_id == STALL_BUFFER_ID:
                stall_detected = True
                continue

            # Check if this is a debug structure event
            if subevent != DEBUG_SUBEVENT or buffer_id != DEBUG_STRUCT_BUFFER_ID:
                continue

            # Accumulate data
            debug_data.extend(data)
            segment_count += 1

            print(f"Found debug segment {segment_count}: {len(data)} bytes (structure #{len(debug_structures) + 1})")

            # Check if this is the last segment
            if last_segment == 1:
                print(f"Debug structure #{len(debug_structures) + 1} complete: {len(debug_data)} bytes in {segment_count} segments\n")
                debug_structures.append(bytes(debug_data))
                # Reset for next debug structure
                debug_data = bytearray()
                segment_count = 0

    if len(debug_structures) == 0:
        print(f"\nNo debug structure found in BTSNOOP file.", file=sys.stderr)

    return debug_structures, stall_detected

def main():
    parser = argparse.ArgumentParser(description='Parse and analyze NBU debug structure')
    parser.add_argument('input_file', help='Input file containing debug structure')
    parser.add_argument('--format', choices=['bin', 'hex', 'btsnoop'], default='bin',
                       help='Input file format (default: bin)')

    args = parser.parse_args()

    try:
        if args.format == 'bin':
            data_list = [read_binary_file(args.input_file)]
            stall_detected = False # Not supported for this format
        elif args.format == 'hex':
            data_list = [read_hex_file(args.input_file)]
            stall_detected = False # Not supported for this format
        elif args.format == 'btsnoop':
            data_list, stall_detected = extract_debug_from_btsnoop(args.input_file)

        if len(data_list) == 0:
            print("No debug structure available to parse.", file=sys.stderr)
        else:
            # Parse and analyze each debug structure
            for i, data in enumerate(data_list, 1):
                if len(data) == 0:
                    print(f"Error: Debug structure #{i} is empty", file=sys.stderr)
                    continue

                if len(data_list) > 1:
                    print(f"\n{'='*70}")
                    print(f"Analyzing Debug Structure #{i} of {len(data_list)}")
                    print(f"{'='*70}")

                debug_struct = NbuDebugStruct(data)
                analyze_debug_struct(debug_struct)

        # Print stall detection at the end
        if stall_detected:
            print(f"\n{'='*70}")
            print(f"ERROR: NBU is stuck - possible stall or deadlock detected")
            print(f"{'='*70}")

        return 0

    except FileNotFoundError:
        print(f"Error: File '{args.input_file}' not found", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1

if __name__ == '__main__':
    sys.exit(main())

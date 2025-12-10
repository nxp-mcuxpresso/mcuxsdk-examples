'''
* Copyright 2025 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
'''

#!/usr/bin/env python3
"""
Capture BLE HCI packets from serial port and save to BTSNOOP format

Format: [direction(1)][packet_type(1)][packet_data...]
direction: 0x00=TX (host->controller), 0x01=RX (controller->host)
packet_type: 0x01=CMD, 0x02=ACL, 0x04=EVENT, 0x05=ISO
"""

import sys
import struct
import time
import argparse
import serial
from datetime import datetime
import os
import subprocess

# BTSNOOP constants
BTSNOOP_MAGIC = b'btsnoop\x00'
BTSNOOP_VERSION = 1
BTSNOOP_DATALINK_HCI_UART = 1002
PACKET_FLAG_SENT = 0
PACKET_FLAG_RECEIVED = 1

# BLE HCI packet types
HCI_CMD = 0x01      # HCI Command
HCI_ACL = 0x02      # ACL Data (L2CAP)
HCI_EVENT = 0x04    # HCI Event
HCI_ISO = 0x05      # ISO Data (LE Audio)

# Valid packet types for BLE
VALID_BLE_PACKET_TYPES = [HCI_CMD, HCI_ACL, HCI_EVENT, HCI_ISO]

def parse_debug_structures(btsnoop_file):
    """Parse debug structures from BTSNOOP file using debug_struct_parser.py"""

    script_dir = os.path.dirname(os.path.abspath(__file__))
    parser_script = os.path.join(script_dir, 'debug_struct_parser.py')

    if not os.path.exists(parser_script):
        print(f"\nWarning: debug_struct_parser.py not found at {parser_script}")
        return False

    print(f"\n{'='*70}")
    print("Parsing debug structures from capture...")
    print(f"{'='*70}\n")

    try:
        # Run the debug structure parser
        result = subprocess.run(
            [sys.executable, parser_script, btsnoop_file, '--format', 'btsnoop'],
            capture_output=False,
            text=True
        )

        return result.returncode == 0

    except Exception as e:
        print(f"Error running debug structure parser: {e}")
        return False

class HCILogger:
    def __init__(self, port, baudrate, output_file):
        self.port = port
        self.baudrate = baudrate
        self.output_file = output_file
        self.packet_count = 0
        self.ser = None
        self.f_out = None

    def write_btsnoop_header(self):
        """Write BTSNOOP file header"""
        self.f_out.write(BTSNOOP_MAGIC)
        self.f_out.write(struct.pack('>I', BTSNOOP_VERSION))
        self.f_out.write(struct.pack('>I', BTSNOOP_DATALINK_HCI_UART))
        self.f_out.flush()

    def get_timestamp_usec(self):
        """Get current timestamp in BTSNOOP format"""
        BTSNOOP_EPOCH_DELTA = 0x00dcddb30f2f8000
        unix_time_usec = int(time.time() * 1000000)
        return unix_time_usec + BTSNOOP_EPOCH_DELTA

    def write_btsnoop_packet(self, flags, packet_type, packet_data):
        """Write a single packet to BTSNOOP file"""
        timestamp = self.get_timestamp_usec()

        # Full packet includes type byte
        full_packet = bytes([packet_type]) + packet_data
        orig_len = len(full_packet)
        incl_len = orig_len
        drops = 0

        # Write packet record header
        self.f_out.write(struct.pack('>I', orig_len))
        self.f_out.write(struct.pack('>I', incl_len))
        self.f_out.write(struct.pack('>I', flags))
        self.f_out.write(struct.pack('>I', drops))
        self.f_out.write(struct.pack('>Q', timestamp))

        # Write packet data
        self.f_out.write(full_packet)
        self.f_out.flush()

    def get_packet_length(self, packet_type, first_bytes):
        """Get expected packet length based on type and header"""

        if packet_type == HCI_CMD:
            # CMD: OpCode(2) + ParamLen(1) + Params
            if len(first_bytes) < 3:
                return None
            param_len = first_bytes[2]
            return 3 + param_len

        elif packet_type == HCI_ACL:
            # ACL: Handle(2) + DataLen(2) + Data
            if len(first_bytes) < 4:
                return None
            data_len = struct.unpack('<H', first_bytes[2:4])[0]
            return 4 + data_len

        elif packet_type == HCI_EVENT:
            # EVENT: EventCode(1) + ParamLen(1) + Params
            if len(first_bytes) < 2:
                return None
            param_len = first_bytes[1]
            return 2 + param_len

        elif packet_type == HCI_ISO:
            # ISO: Handle(2) + DataLen(2) + Data
            if len(first_bytes) < 4:
                return None
            data_len = struct.unpack('<H', first_bytes[2:4])[0]
            return 4 + data_len

        return None

    def read_packet(self):
        """Read one complete HCI packet from serial port"""

        # Read direction marker
        direction_byte = self.ser.read(1)
        if len(direction_byte) == 0:
            return None

        direction = direction_byte[0]
        if direction not in [0x00, 0x01]:
            print(f"Warning: Invalid direction 0x{direction:02X}, skipping...")
            return None

        # Read packet type
        type_byte = self.ser.read(1)
        if len(type_byte) == 0:
            return None

        packet_type = type_byte[0]
        if packet_type not in VALID_BLE_PACKET_TYPES:
            print(f"Warning: Invalid BLE packet type 0x{packet_type:02X}, skipping...")
            return None

        # Read enough bytes to determine packet length
        if packet_type == HCI_CMD:
            header_size = 3
        elif packet_type == HCI_ACL:
            header_size = 4
        elif packet_type == HCI_EVENT:
            header_size = 2
        elif packet_type == HCI_ISO:
            header_size = 4
        else:
            return None

        # Read header
        header = self.ser.read(header_size)
        if len(header) < header_size:
            print(f"Warning: Incomplete header, got {len(header)} bytes, expected {header_size}")
            return None

        # Get total packet length
        packet_len = self.get_packet_length(packet_type, header)
        if packet_len is None:
            print(f"Warning: Could not determine packet length")
            return None

        # Read remaining data
        remaining = packet_len - header_size
        if remaining > 0:
            data = self.ser.read(remaining)
            if len(data) < remaining:
                print(f"Warning: Incomplete packet data, got {len(data)} bytes, expected {remaining}")
                return None
            packet_data = header + data
        else:
            packet_data = header

        return (direction, packet_type, packet_data)

    def run(self):
        """Main capture loop"""

        try:
            # Open serial port
            print(f"Opening serial port {self.port} at {self.baudrate} baud...")
            self.ser = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1.0
            )

            # Open output file
            print(f"Creating output file {self.output_file}...")
            self.f_out = open(self.output_file, 'wb')

            # Write BTSNOOP header
            self.write_btsnoop_header()

            print(f"Capturing BLE HCI packets... (Press Ctrl+C to stop)")
            print(f"{'Time':<12} {'Dir':<4} {'Type':<6} {'Length':<8} {'Info'}")
            print("-" * 70)

            start_time = time.time()

            # Capture loop
            while True:
                packet = self.read_packet()
                if packet is None:
                    continue

                direction, packet_type, packet_data = packet

                # Convert direction to BTSNOOP flags
                flags = PACKET_FLAG_RECEIVED if direction == 0x01 else PACKET_FLAG_SENT

                # Write to BTSNOOP file
                self.write_btsnoop_packet(flags, packet_type, packet_data)

                self.packet_count += 1

                # Print packet info
                elapsed = time.time() - start_time
                direction_str = "RX" if direction == 0x01 else "TX"

                type_names = {
                    HCI_CMD: "CMD",
                    HCI_ACL: "ACL",
                    HCI_EVENT: "EVT",
                    HCI_ISO: "ISO"
                }
                type_str = type_names.get(packet_type, f"0x{packet_type:02X}")

                # Get packet info
                info = ""
                if packet_type == HCI_CMD and len(packet_data) >= 2:
                    opcode = struct.unpack('<H', packet_data[0:2])[0]
                    info = f"OpCode=0x{opcode:04X}"
                elif packet_type == HCI_EVENT and len(packet_data) >= 1:
                    event_code = packet_data[0]
                    info = f"Event=0x{event_code:02X}"
                elif packet_type == HCI_ACL and len(packet_data) >= 2:
                    handle = struct.unpack('<H', packet_data[0:2])[0] & 0x0FFF
                    info = f"Handle=0x{handle:03X}"
                elif packet_type == HCI_ISO and len(packet_data) >= 2:
                    handle = struct.unpack('<H', packet_data[0:2])[0] & 0x0FFF
                    info = f"Handle=0x{handle:03X}"

                print(f"{elapsed:>11.3f}s {direction_str:<4} {type_str:<6} {len(packet_data):<8} {info}")

        except KeyboardInterrupt:
            print("\n\nCapture stopped by user")

        except Exception as e:
            print(f"\nError: {e}")

        finally:
            # Cleanup
            if self.ser and self.ser.is_open:
                self.ser.close()

            if self.f_out:
                self.f_out.close()

            print(f"\nCapture complete:")
            print(f"  Total packets: {self.packet_count}")
            print(f"  Output file: {self.output_file}")
            print(f"\nYou can now open {self.output_file} in Wireshark")

def main():
    parser = argparse.ArgumentParser(
        description='Capture BLE HCI packets from serial port and save to BTSNOOP format',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Supported BLE HCI packet types:
  0x01 - HCI Command (Host → Controller)
  0x02 - ACL Data (Bidirectional L2CAP data)
  0x04 - HCI Event (Controller → Host)
  0x05 - ISO Data (LE Audio, bidirectional)

Examples:
  # Capture from COM3 at 115200 baud
  python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop
  
  # Capture from /dev/ttyUSB0 at 921600 baud
  python hci_to_btsnoop.py -p /dev/ttyUSB0 -b 921600 -o capture.btsnoop
  
  # Auto-generate filename with timestamp
  python hci_to_btsnoop.py -p COM3 -b 115200
        '''
    )

    parser.add_argument('-p', '--port', 
                        required=True,
                        help='Serial port (e.g., COM3, /dev/ttyUSB0)')
    parser.add_argument('-b', '--baudrate', 
                        type=int, 
                        default=115200,
                        help='Baudrate (default: 115200)')
    parser.add_argument('-o', '--output', 
                        help='Output BTSNOOP file (default: hci_capture_TIMESTAMP.btsnoop)')
    parser.add_argument('--parse-debug',
                        action='store_true',
                        help='Automatically parse debug structures after capture')

    args = parser.parse_args()

    # Generate output filename if not provided
    if not args.output:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        args.output = f"hci_capture_{timestamp}.btsnoop"

    # Create logger and run
    logger = HCILogger(args.port, args.baudrate, args.output)
    logger.run()

    # Auto-parse debug structures if requested
    if args.parse_debug and os.path.exists(args.output):
        parse_debug_structures(args.output)

if __name__ == '__main__':
    main()

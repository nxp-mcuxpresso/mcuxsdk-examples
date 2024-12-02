# dsc_crc

## Overview
The CRC Example project is a demonstration program that uses the KSDK software to generate checksums
for an ASCII string. Several CRC protocols are implemented using the CRC driver API.

## Running the demo
Example output on terminal:

CRC Peripheral Driver Example.

Test same string base on different CRC protocol.
 test string: 123456789
 1.CRC-16 CCIT FALSE: 0x29b1
 2.CRC-16 KERMIT: 0x2189
 3.CRC-16 MAXIM: 0x44c2
 4.CRC-32: 0xcbf43926
 5.CRC-32 POSIX: 0x765e7680

Test different string based on CRC-32 POSIX protocol.
 1.test string: abcdefgh
  CRC-32 POSIX: 0x5024ec61
 2.test string: abcd1234
  CRC-32 POSIX: 0x3fbda0ab

## Supported Boards

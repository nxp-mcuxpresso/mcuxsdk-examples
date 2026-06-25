# xspi_quad_polling_transfer

## Overview
The xspi_quad_polling_transfer example shows how to use the XSPI driver with polling
to access a Quad SPI NOR flash device.
In this example, XSPI will send data and operate the external Quad NOR flash connected
with XSPI. The flash is configured for quad transfers, then the following operations
are performed at multiple transfer sizes (64, 128, 256 bytes):
- Sector Erase (4KB)
- Page Program
- IP Read and verify
- AHB Read and verify

After all tests complete, the flash is reset back to standard SPI mode.

## Expected output
```
XSPI Quad I/O polling example started!
4-byte address mode entered.
Quad I/O mode enabled.
Flash vendor ID: 0xEF

--- Test: 64 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 128 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 256 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

--- Test: 512 bytes ---
  IP read verify: PASS
  AHB read verify: PASS

XSPI Quad I/O polling example finished.
Flash reset to default SPI mode.
```

## Supported Boards
- [FRDM-IMXRT700](../../../../_boards/frdmimxrt700/driver_examples/xspi/quad/polling_transfer/example_board_readme.md)
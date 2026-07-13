# xspi_quad_polling_transfer

## Overview
The xspi_quad_polling_transfer example shows how to use the XSPI driver with polling
to access a Quad SPI NOR flash device in QPI SDR (4-4-4) mode.

In this example, XSPI will send data and operate the external Quad NOR flash connected
with XSPI. The flash is first configured into QPI 4-4-4 mode, then the following
operations are performed at multiple transfer sizes (64, 128, 256 bytes):
- Sector Erase (4KB)
- Page Program (4-4-4)
- IP Read and verify (4-4-4)
- AHB Read and verify (4-4-4)

After all tests complete, the flash is reset back to standard SPI mode.

## Expected output
```
XSPI QPI SDR (4-4-4) polling example started!
4-byte address mode entered.
QPI (4-4-4) mode enabled.
Flash vendor ID: 0xEF

--- Test size: 64 bytes ---
  IP read: PASS
  AHB read: PASS

--- Test size: 128 bytes ---
  IP read: PASS
  AHB read: PASS

--- Test size: 256 bytes ---
  IP read: PASS
  AHB read: PASS

XSPI QPI SDR (4-4-4) polling example finished.
Flash reset to default SPI mode.
```

## Supported Boards
- [FRDM-IMXRT700](../../../../_boards/frdmimxrt700/driver_examples/xspi/quad/polling_transfer/example_board_readme.md)
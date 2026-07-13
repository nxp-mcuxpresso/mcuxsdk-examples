# xspi_quad_edma_transfer

## Overview
The xspi_quad_edma_transfer example shows how to use the XSPI driver with EDMA
to access a Quad SPI NOR flash device in QPI SDR (4-4-4) mode.

In this example, XSPI will send data and operate the external Quad NOR flash connected
with XSPI. The flash is first configured into QPI 4-4-4 mode, then the following
operations are performed:
- Sector Erase (4KB)
- Page Program (4-4-4) via EDMA
- EDMA Read and verify (4-4-4)
- AHB Read and verify (4-4-4)

After all tests complete, the flash is reset back to standard SPI mode.

## Expected output
```
XSPI QPI SDR (4-4-4) EDMA example started!
4-byte address mode entered.
QPI (4-4-4) mode enabled.
Flash vendor ID: 0xEF
Erasing Serial NOR over XSPI...
Erase done.
Erase verify (EDMA read) - OK.
Page program done.
==> EDMA program+read: PASS.
==> AHB program+read: PASS.
XSPI QPI SDR (4-4-4) EDMA example finished.
Flash reset to default SPI mode.
```

## Supported Boards
- [FRDM-IMXRT700](../../../../_boards/frdmimxrt700/driver_examples/xspi/quad/edma_transfer/example_board_readme.md)
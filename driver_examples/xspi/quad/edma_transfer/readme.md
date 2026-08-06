# xspi_quad_edma_transfer

## Overview
The xspi_quad_edma_transfer example shows how to use the XSPI driver with EDMA
to access a Quad SPI NOR flash device.

In this example, XSPI will send data and operate the external Quad NOR flash connected
with XSPI. Then the following
operations are performed:
- Sector Erase (4KB)
- Page Program via EDMA
- EDMA Read and verify
- AHB Read and verify

After all tests complete, the flash is reset back to standard SPI mode.

## Expected output
```
XSPI EDMA example started!
Flash vendor ID: 0xEF
Erasing Serial NOR over XSPI...
Erase done.
Erase verify (EDMA read) - OK.
Page program done.
==> EDMA program+read: PASS.
==> AHB program+read: PASS.
XSPI EDMA example finished.
Flash reset to default SPI mode.
```

## Supported Boards
- [FRDM-IMXRT700](../../../../_boards/frdmimxrt700/driver_examples/xspi/quad/edma_transfer/example_board_readme.md)
- [MIMXRT2660EVK](../../../../_boards/mimxrt2660evk/driver_examples/xspi/quad/edma_transfer/example_board_readme.md)

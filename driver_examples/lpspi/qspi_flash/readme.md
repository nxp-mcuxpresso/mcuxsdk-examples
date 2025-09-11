# lpspi_qspi_flash

## Overview
This example shows how to use LPSPI to control the QSPI flash, such as erasing,
programing, reading.

Accessing the QSPI flash requires using the CS continuous mode. To enable
this mode with LPSPI, dynamic adjustment of the TCR[CONT] and TCR[CONTC]
is necessary at specific timing during the data transfer.
For example, TCR[CONT]=1 at the first frame, TCR[CONTC]=1 at
frame boundaries, and TCR[CONT]=0 for the final frame. To simplify control flow,
this project does not use LPSPI's CS continuous mode. Instead, it manages
the CS signal via GPIO in coordination with LPSPI.

When the project run finished, you can see the pass or fail information from
the terminal.

## Supported Boards
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/lpspi/qspi_flash/example_board_readme.md)

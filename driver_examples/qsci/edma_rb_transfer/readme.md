# qsci_edma_rb_transfer

## Overview
The qsci_edma_rb_transfer example demonstrates the usage of the KSDK QSCI EDMA driver. 
In this example, a ring buffer is used with EDMA enabled, data received will be stored
in ring buffer first, while IDLE line is detected, the received characters will be read out,
and echo them back to the terminal using EDMA method.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/qsci/edma_rb_transfer/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/qsci/edma_rb_transfer/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/qsci/edma_rb_transfer/example_board_readme.md)

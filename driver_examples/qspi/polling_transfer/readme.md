# qspi_polling_transfer

## Overview
The qspi_polling example shows how to use qspi driver with polling:

In this example, qspi will send data and operate the external flash connected with QSPI. Some simple flash command will
be executed, such as Write Enable, Erase sector, Program page.
Example will first erase the sector and programe a page into the flash, at last check if the data in flash is correct.

Notice: While do program, it is not suggested to make the flash write frequency bigger than the core clock frequency.
In some tool chain's Debug version, this may cause core do not have enough speed to send data to flash.
The flash write frequency equals to QSPI working frequency plus data line number. For example, if QSPI working in 48MHz,
program command uses quad mode, the write frequency is 48MHz * 4 = 192MHz.

## Supported Boards
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/qspi/polling_transfer/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/driver_examples/qspi/polling_transfer/example_board_readme.md)

# cmsis_ecspi_int_loopback_transfer

## Overview
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage method please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_ecspi_int_loopback_transfer example shows how to use CMSIS ECSPI driver in interrupt way:
 
In this example , ECSPI will do a loopback transfer in interrupt way, so, there is no need to set up any pins.
And we should set the ECSPIx->TESTREG[LBC] bit, this bit is used in Master mode only. When this bit is set, 
the ECSPI connects the transmitter and receiver sections internally, and the data shifted out from the 
most-significant bit of the shift register is looped back into the least-significant bit of the Shift register.
In this way, a self-test of the complete transmit/receive path can be made. The output pins are not affected, 
and the input pins are ignored.

## Supported Boards
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/cmsis_driver_examples/ecspi/int_loopback_transfer/example_board_readme.md)
- [EVK-MIMX8MN](../../../_boards/evkmimx8mn/cmsis_driver_examples/ecspi/int_loopback_transfer/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../../_boards/evkmimx8mnddr3l/cmsis_driver_examples/ecspi/int_loopback_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/cmsis_driver_examples/ecspi/int_loopback_transfer/example_board_readme.md)
- [EVK-MIMX8MQ](../../../_boards/evkmimx8mq/cmsis_driver_examples/ecspi/int_loopback_transfer/example_board_readme.md)

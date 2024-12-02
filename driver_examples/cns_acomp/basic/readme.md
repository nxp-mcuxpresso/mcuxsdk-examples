# cns_acomp_basic

## Overview
The acomp basic driver example demostrates the basic usage of the ACOMP module. This example compares the user input
analog signal with interanl reference voltage(VDDIO_3 * 0.5) and will toggle the LED when the result changes. The purpose
of this demo is to show how to use the ACOMP driver in SDK software. In this driver example the 'outPinMode' is set
as 'kACOMP_PinOutSynInverted', so the output signal from gpio is the inversion of the comparator actual output signal.

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/acomp/basic/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/acomp/basic/example_board_readme.md)

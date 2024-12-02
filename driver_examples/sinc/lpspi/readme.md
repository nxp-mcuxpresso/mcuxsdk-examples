# sinc_lpspi

## Overview
The SINC LPSPI example demonstrates how to use the SINC driver to convert SPI output bitstreams to a data stream.
The LPSPI output the default value continuously, and the SINC module is connected to the LPSPI's output to resume default data.
The SINC module works in trigger mode, and once a user inputs any key via the terminal, one SINC conversion will be triggered.
The bitstream source is set as an external bitstream that connects to the SPI MOSI signal, and the input clock source is set as MCK_OUT0.
Theoretically, the triggered SINC result should be equal to the SPI's output value, but there may be some deviations between different conversions.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/sinc/lpspi/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/sinc/lpspi/example_board_readme.md)

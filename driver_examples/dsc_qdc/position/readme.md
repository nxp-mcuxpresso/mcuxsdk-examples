# qdc_position

## Overview

This project shows how to sample the shaft position based on quadrature signal (PHASEA and PHASEB).
This project assumes that the shaft encoder disk has 1000 lines. To count the revolution,
QDC modulo count mode is enabled, and modulus value set to 4x1000. User could also change
to use the INDEX signal to count revolution.

This project setups a 1kHz signal as TRIGGER signal, the QDC position, revolution,
and position difference are loaded to HOLD registers at the edge of TRIGGER signal.
In the TRIGGER signal's ISR, the hold position values are saved to global variables,
user can check the variables through freemaster. Also, these global variables
are output to the debug terminal every 500ms.

## Supported Boards
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/qdc/position/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/qdc/position/example_board_readme.md)

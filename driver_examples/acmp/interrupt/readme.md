# acmp_interrupt

## Overview
The ACMP Interrupt project is a simple demonstration program that uses the SDK software. It
compares the selected analog input with ACMP internal DAC output continuously and toggle the LED
when the final comparison result changed. The purpose of this demo is to show how to use the
ACMP driver in SDK software by interrupt way. The ACMP can be configured based on default
configuration returned by the API ACMP_GetDefaultConfig(). The default configuration is: high
speed is not enabled, invert output is not enabled, unfiltered output is not enabled, pin out
is not enabled, offset level is level 0, hysteresis level is level 0.

## Running the demo
Before example running, please connect the analog input voltage to stable external voltage generator
to avoid floating voltage and adjust external voltage generator to be higher than reference DAC output
(half of referene voltage) to make the initial comparison result to be 0.

When the demo runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).
The LED will be turned ON/OFF when the analog input is LOWER/HIGHER than the DAC output.
Change the analog input voltage to see the LED status.
The analog input is LOWER than DAC output
The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/acmp/interrupt/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/acmp/interrupt/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/acmp/interrupt/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/acmp/interrupt/example_board_readme.md)

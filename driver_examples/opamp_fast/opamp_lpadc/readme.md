# opamp_fast_lpadc

## Overview

The OPAMP lpadc example demonstrates how to use the OPAMP driver.
In this example, the reference voltage (Vref) of the OPAMP1
non-inverting input port comes from the output port of OPAMP0,
which needs to be connected to an LPADC channel, the OPAMP1
non-inverting input port needs to be connected to a voltage
source and an LPADC channel, the OPAMP1 inverting input port
needs to be connected to GND, and the OPAMP1 output port needs
to be connected to an LPADC channel.

When this example runs successfully, the terminal will print the
OPAMP0 output port voltage value, the OPAMP1 non-inverting input
port voltage value, the OPAMP1 output voltage value measured using
lpadc, and the theoretical OPAMP1 output value.

The theoretical calculated value should be very close to the value
measured by LPADC.

OPAMP1 input and output relationship formula: Vout = Vref + (R2 / R1 * (Vp - Vn)).

## Supported Boards
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/opamp/opamp_lpadc/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/opamp/opamp_lpadc/example_board_readme.md)

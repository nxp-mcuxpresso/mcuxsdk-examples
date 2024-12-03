# aoi_input_mux

## Overview
The INPUT_MUX AOI demo application demonstrates the use of CMP and SCTIMER AOI peripherals.
The example Use the output of cmp and sctimer as the input of the aoi function. When the input voltage of cmp changes,
The output of cmp changes, which changes the output of aoi.  
The purpose of this demonstration is to show how to use the AOI driver in the SDK software.

## Run the demo
The VREF voltage is half of the reference voltage.

After the demonstration runs successfully, you can see the following information on the oscilloscope:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMP input voltage is less than the VREF voltage, AOI has no waveform output
CMP input voltage is greater than the VREF voltage, AOI continuous output square wave
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/aoi/input_mux/example_board_readme.md)

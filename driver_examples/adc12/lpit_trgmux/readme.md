# adc12_lpit_trgmux

## Overview

The adc12_lpit_trgmux example shows how to use the LPIT and TRGMUX to generate a ADC trigger.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC12's
sample input. When run the example, the ADC is triggered by TRGMUX and gets the ADC conversion result in the ADC
Conversion Complete (COCO) Interrupt. The LPIT is configured as periodic counter which will output pre-trigger and
tigger signal to TRGMUX periodically.

## Running the demo
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADC Full Range: 4096

Change the input voltage to check ADC12's conversion result...

ADC12 Interrupt Counter: 1
ADC12 Conversion Completed, result value: 4056

ADC12 Interrupt Counter: 2
ADC12 Conversion Completed, result value: 781
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/adc12/lpit_trgmux/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/adc12/lpit_trgmux/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/adc12/lpit_trgmux/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/adc12/lpit_trgmux/example_board_readme.md)

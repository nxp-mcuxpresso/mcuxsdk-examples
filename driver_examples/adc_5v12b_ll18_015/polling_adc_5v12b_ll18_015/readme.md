# adc_5v12b_ll18_015_polling

## Overview
The adc_polling example shows the simplest way to use adc driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the adc's
sample input. When running the project, typing any key into debug console would trigger the conversion. The execution 
would check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. 
Then read the conversion result value and print it to debug console.

## Supported Boards
- [FRDM-KE02Z40M](../../../_boards/frdmke02z40m/driver_examples/adc/polling/example_board_readme.md)

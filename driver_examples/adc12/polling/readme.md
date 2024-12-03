# adc12_polling

## Overview

The adc12_polling example shows the simplest way to use ADC12 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC12's sample input.
When running the project, typing any key into debug console would trigger the conversion. The execution would check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. Then read the conversion result value and print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change the
configuration structure's setting in application to fit the special requirement.

The auto-calibration is not essential but strongly recommended. It can help to adjust the converter itself and improve the
ADC12's performance.

## Running the demo
When the example runs successfully, following information can be seen on the OpenSDA terminal:
ADC12 polling Example.
ADC Full Range: 256
Press any key to get user channel's ADC value ...

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/adc12/polling/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/adc12/polling/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/adc12/polling/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/adc12/polling/example_board_readme.md)

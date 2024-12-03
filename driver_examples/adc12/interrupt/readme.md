# adc12_interrupt

## Overview

The adc12_interrupt example shows how to use interrupt with ADC12 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC12's sample input.
When running the project, typing any key into debug console would trigger the conversion. ADC interrupt would be asserted once the conversion is completed. In ADC ISR, the conversion completed flag would be cleared by reading the conversion result value.
Also, the conversion result value is stored, and the ISR counter is increased. These information would be printed when the execution return to the main loop.

The point is that the ADC12 interrupt configuration is set when configuring the ADC12's conversion channel. When in software trigger mode, the conversion would be launched by the operation of configuring channel, just like writing a conversion command. So if user wants to generate the interrupt every time the conversion is completed, the channel's configuration with enabling interrupt setting would be used for each conversion.

## Running the demo
When the example runs successfully, following information can be seen on the OpenSDA terminal:
ADC12 interrupt Example.
ADC Full Range: 256
Press any key to get user channel's ADC value ...

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/adc12/interrupt/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/adc12/interrupt/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/adc12/interrupt/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/adc12/interrupt/example_board_readme.md)

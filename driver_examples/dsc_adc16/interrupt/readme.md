# dsc_adc16_interrupt

## Overview

The adc16_interrupt example shows how to use interrupts through the ADC16 driver.

In this example, the user should specify a channel to provide a voltage signal (which can be controlled by the user) as the ADC16
Sample input. When the project is running, any key input in the debug console will trigger the conversion. ADC interrupt
Once the conversion is complete, it will be asserted. Once the value of the conversion data register is read in the interrupt function,
the conversion flag is cleared and one conversion ends. When the conversion is performed again, and the conversion flag is set to 1, 
the conversion result is placed in the data register.

The important point is to set the ADC16 interrupt configuration when configuring the ADC16 conversion channel. When
Software trigger mode, start the conversion through the operation of the configuration channel, just like writing a
Conversion command. So if the user wants to generate an interrupt every time the conversion is completed, the channel’s
Each conversion will use the configuration with interrupt settings enabled

## Supported Boards

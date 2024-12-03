# adc_dma

## Overview

The qn_adc_dma example shows how to use QN ADC driver in the simplest way.

In this example, user should provide a voltage(from 0v to 2.7v) signal on the PA08 pin as the ADC's sample input. When user type in any key from the keyboard, the software trigger and DMA start API is called to start the conversion. Then it polls the conversion completed flag till the conversion is completed. Then read the conversion result value and print it to debug console.

## Running the demo
When the example runs successfully, following information can be seen on the OpenSDA terminal:
 
ADC dma example.
Configuration Done.
Press any key to get demo channel's ADC value ...
Original: 0x401e9081    Ch: 8   Result: 1800.470825(mv)
Original: 0x401e8eb5    Ch: 8   Result: 1800.265625(mv)
Original: 0x401e8e85    Ch: 8   Result: 1800.244141(mv)
Original: 0x401e8e21    Ch: 8   Result: 1800.199707(mv)

...

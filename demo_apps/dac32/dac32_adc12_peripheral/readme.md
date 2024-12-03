# dac32_adc12_peripheral

## Overview
The dac32_adc12 example shows a simple case of using DAC32 and ADC12. The DAC32 module would output analog signal, then ADC12 module would read this value of this signal and output the voltage value.

User needs to make sure that DAC's output and ADC's input are stable.
Notice:In few cases of this demo,DAC's input and ADC's output might not match,since the voltage signal
is unstable.User has to wait the DAC a few while to make the result as expected in application if necessary.

# adc_polling

## Overview
The SAR ADC polling example shows the simplest way to use the SAR ADC driver.

In this example, three channels (channel 0, 1, and 2) are used to form a conversion chain, 
these three channels are executed in the normal conversion one-shot mode. The end-of-channel
interrupt, pre-sample, DMA transfer, and watchdog features of each channel in the conversion
chain are disabled. The user should provide a voltage signal to each conversion channel. When
running the project, type any key into the debug console to get these three channels' conversion
result, if the result of the channel conversion is valid, it will be printed to the debug console.


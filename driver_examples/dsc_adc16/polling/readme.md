# dsc_adc16_polling

## Overview

The adc16_polling example shows the simplest way to use ADC16 driver.

In this example, user should indicate a channel to provide a voltage signal (can be controlled by user) as the ADC16's
sample input. When running the project, typing any key into debug console would trigger the conversion. The execution 
would check the conversion completed flag in loop until the flag is asserted, which means the conversion is completed. 
Then read the conversion result value and print it to debug console.

Note, the default setting of initialization for the ADC converter is just an available configuration. User can change
the configuration structure's setting in application to fit the special requirement.

The auto-calibration is not essential but strongly recommended. It can help to adjust the converter itself and improve
the ADC16's performance.

## Supported Boards

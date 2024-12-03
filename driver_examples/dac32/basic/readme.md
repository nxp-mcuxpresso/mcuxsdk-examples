# dac32_basic

## Overview

The dac32_basic example shows how to use DAC32 module simply as the general DAC converter.

When the DAC32's buffer feature is not enabled, the first item of the buffer is used as the DAC output data register.
The converter would always output the value of the first item. In this example, it gets the value from terminal,
outputs the DAC output voltage through DAC output pin.

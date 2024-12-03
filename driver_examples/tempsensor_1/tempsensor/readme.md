# tempsensor

## Overview
The tempsensor example simply shows how to use the tempsensor driver APIs in the SDK.

In this example, the tempsensor works in continuous mode, using threshold 2 to monitor the minimum
die temperature, set the threshold 2 value to the user-specified minimum die temperature (using 
Celsius uint), set the threshold 2 comparator mode to mode 0 (less than or equal to the threshold),
and enable the threshold 2 interrupt. 

When the program runs successfully, the serial port terminal will print out the corresponding 
information. The user can press any button on the PC to see the measured temperature value. Use
some methods to cool down the chip (such as air cooling). When the chip temperature is lower 
than or equal to the threshold 2 value, a corresponding interrupt will be generated and relevant 
information will be printed to the terminal. 

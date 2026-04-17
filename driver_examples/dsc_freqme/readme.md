# freqme_interrupt

## Overview
The freqme_interrupt is a demonstration program of the SDK DSC_FREQME driver's features.  
The example demonstrates the usage of frequency-measurement operating mode and pulse-width measurement operating mode.  

In frequency measurement mode, the reference clock source is fixed. Users can select target clock source and input
reference clock scaling factor, then after a while, the frequency of target clock source will be printed to the
terminal. To measure the frequency with a high degree of accuracy, the frequency of target clock is better less than
that of the reference clock.  

In pulse width measurement mode, the reference clock source is fixed. Users can select target clock source and pulse
polarity, then after a while, the high or low period of reference clock will be printed to the terminal.   

FREQME uses the known high or low period of the reference clock as a measurement window.  
During this window, it counts the number of pulses from the target clock.  
By combining the known duration of the reference clock's high or low period with the counted target pulses, the target clock frequency can be calculated.  
If the reference clock has a high period of 1 ms and 1000 target pulses occur during that time, the target frequency is 1 MHz.  

To measure the pulse period with a high degree of accuracy, the frequency of reference clock is better less than that of target clock.

## Supported Boards

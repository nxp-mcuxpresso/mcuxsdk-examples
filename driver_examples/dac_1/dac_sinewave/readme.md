# dac_sinewave

## Overview
The dac_basic example shows how to use DAC module to generate sinewave.

The user can specify the sinewave frequency and how many output points
in one cycle of the sinewave. Based on these two parameters and the
functional clock frequency of ctimer, we can calculate how long each
interval enters the ctimer interrupt.

Note: If the device does not have an FPU, then running the example directly
may get bad performance, and the user can add an array, the elements of the
array are the input values corresponding to the DAC output voltage values 
for one sinewave cycle.

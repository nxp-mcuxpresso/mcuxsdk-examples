# tsc_debugMode

## Overview
The fourWireModeInterrupt example shows how to use TSC driver in 4-wire debug modes:
The touch screen controller needs to co-work with ADC and TSC analogue.

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, before TSC starts work, ADC driver configure ADC_HCx;
2. Then, we configure TSC module to work in debug mode.
3. Then, we configure six states(1st-pre-charge, 1st-detect, x-measure, y-measure, and 
   2nd-pre-charge, 2nd-detect) to simulate non-debug(hardware) mode.
4. Lastly, if measure data is valid, serial terminal prints x/y-coordinate information.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSC DebugMode Example Start!
ADC_5HC_DoAutoCalibration() Done.
Please touch screen!
x = 234, y = 3655
x = 239, y = 3715
x = 236, y = 3708
x = 236, y = 3667
x = 236, y = 3653
x = 236, y = 3628
x = 233, y = 3637
x = 236, y = 3680
...

## Supported Boards

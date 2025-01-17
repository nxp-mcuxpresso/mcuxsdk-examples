# tsc_fourWireModeInterrupt

## Overview
The fourWireModeInterrupt example shows how to use TSC driver in 4-wire interrupt modes:
The touch screen controller needs to co-work with ADC and TSC analogue.

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, before TSC starts work, ADC driver configure ADC_HCx;
2. Then, we configure TSC module to work in non-auto-measure mode.
3. Then, we start screen detection and wait for software order after detects a touch (and generates an interrupt);
4. Then, we start measure during detect interruption.
5. Then, we judge whether the measure data is valid during measure interruption.
6. Lastly, if measure data is valid, serial terminal prints x/y-coordinate information.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSC fourWireModeInterrupt Example Start!
ADC_5HC_DoAutoCalibration() Done.
Please touch screen.
x = 3741, y = 1068
x = 3741, y = 572
x = 3746, y = 517
x = 3745, y = 518
x = 3751, y = 508
x = 3752, y = 512
x = 3750, y = 507
...

## Supported Boards

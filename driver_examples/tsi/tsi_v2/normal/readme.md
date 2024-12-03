# tsi_v2_normal

## Overview
The tsi_v2_normal example shows how to use TSI_V2 driver in normal modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSI_V2 Normal_mode Example Start!
Calibrated counters for channel 0 is: 272 
Calibrated counters for channel 1 is: 1209 
Calibrated counters for channel 2 is: 45985 
Calibrated counters for channel 3 is: 2235 
Calibrated counters for channel 4 is: 35656 
Calibrated counters for channel 5 is: 652 
Calibrated counters for channel 6 is: 353 
Calibrated counters for channel 7 is: 544 
Calibrated counters for channel 8 is: 582 
Calibrated counters for channel 9 is: 715 
Calibrated counters for channel 10 is: 385 
Calibrated counters for channel 11 is: 339 
Calibrated counters for channel 12 is: 371 
Calibrated counters for channel 13 is: 350 
Calibrated counters for channel 14 is: 416 
Calibrated counters for channel 15 is: 421 

NOW, comes to the software trigger scan using polling method!
Channel 5 Normal mode counter is: 649 
Channel 8 Normal mode counter is: 582 

NOW, comes to the software trigger scan using interrupt method!
Channel 5 Normal mode counter is: 650 
Channel 8 Normal mode counter is: 582 

NOW, comes to the periodical scan method!
After running, touch pad D9 or D8, you will see LED shines.
# tsi_v5_self_cap

## Overview
The tsi_v5_self_cap example shows how to use TSI_V5 driver in self-cap mode:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSI_V5 Self-Cap mode Example Start!
Calibrated counters for channel 0 is: 65535 
Calibrated counters for channel 1 is: 65535 
Calibrated counters for channel 2 is: 65535 
Calibrated counters for channel 3 is: 65535 
Calibrated counters for channel 4 is: 65535 
Calibrated counters for channel 5 is: 65535 
Calibrated counters for channel 6 is: 65535 
Calibrated counters for channel 7 is: 65535 
Calibrated counters for channel 8 is: 65535 
Calibrated counters for channel 9 is: 65535 
Calibrated counters for channel 10 is: 65535 
Calibrated counters for channel 11 is: 65535 
Calibrated counters for channel 12 is: 65535 
Calibrated counters for channel 13 is: 65535 
Calibrated counters for channel 14 is: 65535 
Calibrated counters for channel 15 is: 65535 
Calibrated counters for channel 16 is: 599 
Calibrated counters for channel 17 is: 65535 
Calibrated counters for channel 18 is: 65535 
Calibrated counters for channel 19 is: 65535 
Calibrated counters for channel 20 is: 1280 
Calibrated counters for channel 21 is: 1267 
Calibrated counters for channel 22 is: 6387 
Calibrated counters for channel 23 is: 5406 
Calibrated counters for channel 24 is: 65535 

NOW, comes to the software trigger scan using polling method!
Channel 22 Normal mode counter is: 6387 
Channel 23 Normal mode counter is: 5412 

NOW, comes to the software trigger scan using interrupt method!
Channel 22 Normal mode counter is: 6389 
Channel 23 Normal mode counter is: 5407 

NOW, comes to the hardware trigger scan method!
After running, touch pad E1 each time, you will see LED toggles.

## Supported Boards
- [FRDM-KE15Z](../../../../_boards/frdmke15z/driver_examples/tsi_v5/self_cap/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/driver_examples/tsi_v5/self_cap/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/driver_examples/tsi_v5/self_cap/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/driver_examples/tsi_v5/self_cap/example_board_readme.md)

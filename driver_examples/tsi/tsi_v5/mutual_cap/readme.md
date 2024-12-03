# tsi_v5_mutual_cap

## Overview
The tsi_v5_mutual_cap example shows how to use TSI_V5 driver in mutual-cap mode:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSI_V5 Mutual-Cap mode Example Start!

Please do not touch pad KEY3 when in calibration process!
Calibrated counters for mutual-cap pad KEY3 is: 2198 

NOW, comes to the software trigger scan using polling method!
Mutual-cap pad KEY3 Normal mode counter is: 2200 

NOW, comes to the software trigger scan using interrupt method!
Mutual-cap pad KEY3 Normal mode counter is: 2198 

NOW, comes to the hardware trigger scan method!
After running, touch pad KEY3 each time, you will see LED toggles.

## Supported Boards
- [FRDM-KE15Z](../../../../_boards/frdmke15z/driver_examples/tsi_v5/mutual_cap/example_board_readme.md)
- [FRDM-KE16Z](../../../../_boards/frdmke16z/driver_examples/tsi_v5/mutual_cap/example_board_readme.md)
- [FRDM-KE17Z](../../../../_boards/frdmke17z/driver_examples/tsi_v5/mutual_cap/example_board_readme.md)
- [FRDM-KE17Z512](../../../../_boards/frdmke17z512/driver_examples/tsi_v5/mutual_cap/example_board_readme.md)

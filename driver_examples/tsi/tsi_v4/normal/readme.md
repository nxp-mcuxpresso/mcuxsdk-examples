# tsi_v4_normal_mode

## Overview
The tsi_v4_normal example shows how to use TSI_V4 driver in normal modes:

In this example , we make use of the available electrodes on board to show driver usage.
1. Firstly, we get the non-touch calibration results as baseline electrode counter;
2. Then, we start the Software-Trigger scan using polling method and interrupt method;
3. Then, we start the Hardware-Trigger scan using interrupt method.

## Running the demo
When running successfully, the log output in terminal shall be similar as below:

TSI_V4 Normal_mode Example Start!
Calibrated counters for channel 0 is: 2769
Calibrated counters for channel 1 is: 10
Calibrated counters for channel 2 is: 65535
Calibrated counters for channel 3 is: 65535
Calibrated counters for channel 4 is: 1273
Calibrated counters for channel 5 is: 65535
Calibrated counters for channel 6 is: 2955
Calibrated counters for channel 7 is: 2640
Calibrated counters for channel 8 is: 2680
Calibrated counters for channel 9 is: 2880
Calibrated counters for channel 10 is: 2906
Calibrated counters for channel 11 is: 2705
Calibrated counters for channel 12 is: 2947
Calibrated counters for channel 13 is: 5073
Calibrated counters for channel 14 is: 421
Calibrated counters for channel 15 is: 5072

NOW, comes to the software trigger scan using polling method!
Channel xx Normal mode counter is: xxxx
Channel xx Normal mode counter is: xxxx

NOW, comes to the software trigger scan using interrupt method!
Channel xx Normal mode counter is: xxxx
Channel xx Normal mode counter is: xxxx

NOW, comes to the hardware trigger scan method!
After running, touch pad xx each time, you will see LED toggles.

## Supported Boards
- [FRDM-K32L2A4S](../../../../_boards/frdmk32l2a4s/driver_examples/tsi_v4/normal/example_board_readme.md)

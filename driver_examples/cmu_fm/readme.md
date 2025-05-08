# cmu_fm_interrupt

## Overview
The cmu_fm driver example is to demonstrate usage of the SDK CMU_FM driver with interrupt callback functions. In this example, the minimum number of reference clock cycles is calculated as metering window. The CMU_FM count the target clock cycles and generate an interrupt within the given reference clock cycles. The expected and the measured frequency of the monitored clock are printed out in the terminal.

## Supported Boards
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/driver_examples/cmu_fm/example_board_readme.md)
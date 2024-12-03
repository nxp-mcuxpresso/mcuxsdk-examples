# dac14_interrupt

## Overview

The DAC14 interrupt example shows how to use the DAC FIFO interrupt. 
When the application starts to run, it will immediately enter the DAC
ISR and write data into the FIFO which is empty at first. Once the DAC 
FIFO is triggered in the while loop, the data in it shall be read out; 
then the FIFO becomes empty again, and get another round of feed in the 
DAC ISR.

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/dac14/dac14_interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/dac14/dac14_interrupt/example_board_readme.md)

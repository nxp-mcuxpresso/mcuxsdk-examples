# dac_1_buffer_interrupt

## Overview

The dac_buffer_interrupt example shows how to use DAC FIFO interrupt.

When the DAC FIFO empty interrupt is enabled firstly, the application would enter the DAC ISR immediately, since the FIFO is actually empty. Then the FIFO would be feed inside the ISR. Then the DAC interrupt could be restrained. Once the DAC FIFO is triggered in while loop, the data in FIFO is read out, then it becomes empty, so the FIFO would be feed again in DAC ISR. 

With this example, user can define the DAC output array to generate the different wave output. Also the software trigger can be called in some timer ISR so that the DAC would output the analog signal in indicated period. Or even use the hardware trigger to release the CPU.

## Supported Boards
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [FRDM-MCXA346](../../../_boards/frdmmcxa346/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/driver_examples/dac/dac_buffer_interrupt/example_board_readme.md)

# ctimer_dma_trigger

## Overview
This example shows how to use CTimer to trigger DMA transfer by match feature.  
DMA requests are generated when the value of Timer Counter (TC) matches match register.  
In this example, CTimer match event occurs every 1 second and then trigger DMA transfer.  
After DMA transfer two times, that is 2 seconds later, CTimer counter stops and DMA will
not be triggered.  
This example do not generate external outputs.

## Running the demo
The log below shows example output of the CTimer driver simple match demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CTimer trigger DMA memory to memory transfer example begin.
Destination Buffer:
0       0       0       0
CTimer trigger DMA memory to memory transfer example finish.
Destination Buffer:
1       2       3       4
5       6       7       8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/ctimer/dma_trigger/example_board_readme.md)
- [FRDM-MCXL255](../../../_boards/frdmmcxa346/driver_examples/ctimer/simple_pwm_interrupt/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/ctimer/dma_trigger/example_board_readme.md)
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/driver_examples/ctimer/dma_trigger/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/driver_examples/ctimer/dma_trigger/example_board_readme.md)

# ctimer_capture

## Overview
This example shows how to use CTimer to capture the edge. In this example, CTimer
timer counter uses APB clock as clock source, and CTimer monitors capture pin.
The capture pin is connected to a GPIO output. The project pulls GPIO to generate
rising edge. When rising edge detected on the pin, CTimer saves the timer counter value
to capture register, and print in the terminal. The GPIO pin is toggled priodically,
so the edge capture timestamp is shown periodically in the terminal.

## Supported Boards
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/ctimer/capture/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/ctimer/capture/example_board_readme.md)

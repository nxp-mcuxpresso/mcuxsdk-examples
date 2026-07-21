# ctimer_capture

## Overview
This example shows how to use CTimer to capture the edge. In this example, CTimer
timer counter uses APB clock as clock source, and CTimer monitors capture pin.
The capture pin is connected to a GPIO output. The project pulls GPIO to generate
rising edge. When rising edge detected on the pin, CTimer saves the timer counter value
to capture register, and print in the terminal. The GPIO pin is toggled priodically,
so the edge capture timestamp is shown periodically in the terminal.

## Supported Boards
- [FRDM-MCXA346](../../../_boards/frdmmcxa346/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXL255](../../../_boards/frdmmcxa346/driver_examples/ctimer/simple_pwm_interrupt/example_board_readme.md)
- [FRDM-MCXW23](../../../_boards/frdmmcxw23/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/ctimer/capture/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/ctimer/capture/example_board_readme.md)
- [MCXW23-EVK](../../../_boards/mcxw23evk/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA174](../../../_boards/frdmmcxa174/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA344](../../../_boards/frdmmcxa344/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA577](../../../_boards/frdmmcxa577/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXC162](../../../_boards/frdmmcxc162/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA287](../../../_boards/frdmmcxa287/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/ctimer/capture/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXN947T](../../../_boards/frdmmcxn947t/driver_examples/ctimer/capture/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/ctimer/capture/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/ctimer/capture/example_board_readme.md)

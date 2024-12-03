# sctimer_input_capture

## Overview
The SCTimer project is a demonstration program of the SDK SCTimer driver's input capture feature.
This example uses Event 0 to 4, State 0 and 1, Capture 0 to 3, Match 4, one user defined input.

If high level input is detected on the channel when SCTimer timer starts, SCTimer will generate a
capture event incorrectly although there is no rising edge. So this example ignore first pluse and
capture second pluse.
Event 0 and 1 occur in state 0. They are triggered by first rising and falling edge. After event 1
is triggered, new state is state 1. Event 2 and 3 are triggered continuously by following rising
and falling edge in state 1. Event 0,1,2,3 cause Capture 0,1,2,3 individually. SCTimer capture second
pluse in state 1.
Event 4 is used as counter limit(reset). Event 4 causes Match 4, match value is 65535.

This example gets the capture value of the input signal using API SCTIMER_GetCaptureValue() in Event
2 and 3 interrupt.

Need to ensure to input least two pluse into the channel, a pwm signal is recommended.
This example will print the capture values and pluse width of the input signal on the terminal window.

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/sctimer/input_capture/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/sctimer/input_capture/example_board_readme.md)

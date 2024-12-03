# ipwm

## Overview
The PWM project is a simple demonstration program of the SDK PWM driver. It sets up the PWM hardware
block to output PWM signals on one PWM channel. The example also shows PWM duty cycle is increase or decrease.
The FIFO empty interrupt is provided. A new value will be loaded into FIFO when FIFO empty status bit is set. 
The PWM will run at the last set duty-cycle setting if all the values of the FIFO has been utilized, 
until the FIFO is reloaded or the PWM is disabled. When a new value is written, the duty cycle changes after the current period is over.
The outputs can be observed by oscilloscope.

## Supported Boards
- [EVK-MIMX8MM](../../_boards/evkmimx8mm/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMX8MN](../../_boards/evkmimx8mn/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../_boards/evkmimx8mnddr3l/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMX8MP](../../_boards/evkmimx8mp/driver_examples/pwm/example_board_readme.md)
- [EVK-MIMX8MQ](../../_boards/evkmimx8mq/driver_examples/pwm/example_board_readme.md)

# acmp_interrupt

## Overview

In hw_trig example, a counter is used to generate a 10KHz,
70% duty cycle PWM, and a trigger is generated at 50% of
the PWM period. This trigger is routed to bctu as the
hardware trigger source, which is then trigger ADC to
convert.

## Supported Boards
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/bctu/hw_trig/example_board_readme.md)

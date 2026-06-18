# SCTimer Complementary PWM with dead time

## Overview

This example demonstrates the SCTimer driver generating **two center-aligned complementary PWM output
pairs with programmable dead time** on one SCTimer. Both pairs share the same frequency and the unified counter; each pair's duty cycle is then continuously ramped up and down (glitchless, applied at the next period), with pair 2 ramping in the opposite direction to pair 1.

## Supported Boards

- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/sctimer/complementary_pwm/example_board_readme.md)

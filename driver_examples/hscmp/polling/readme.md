# hscmp_polling

## Overview

The HSCMP polling Example shows the simplest way to use HSCMP driver and help user with a quick start.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
HSCMP's positive channel input. On the negative side, the internal 6-bit DAC is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user's voltage crosses the internal DAC's value. The endless loop in main() function
would detect the logic value of comparator's output, and change the LED. The LED would be turned on when the compare
output is logic one, or turned off when zero.

## Supported Boards
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/hscmp/polling/example_board_readme.md)

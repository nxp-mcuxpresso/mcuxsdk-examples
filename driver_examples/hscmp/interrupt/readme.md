# hscmp_interrupt

## Overview

The HSCMP interrupt Example shows how to use interrupt with HSCMP driver.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
HSCMP's positive channel input. On the negative side, the internal 6-bit DAC is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user-defined channel's voltage crosses the internal DAC's value. The change of
comparator's output would generate the falling and rising edge events with their interrupts enabled. When any HSCMP 
interrupt happens, the HSCMP's ISR would turn on the LED light if detecting the output's rising edge, or turn off it when
detecting the output's falling edge.

## Supported Boards
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/hscmp/interrupt/example_board_readme.md)

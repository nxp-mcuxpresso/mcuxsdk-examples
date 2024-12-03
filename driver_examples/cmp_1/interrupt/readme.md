# cmp_1_interrupt

## Overview

The CMP interrupt Example shows how to use interrupt with CMP driver.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
CMP's positive channel input. On the negative side, the internal VREF ladder is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user-defined channel's voltage crosses the internal VREF's value. The change of
comparator's output would generate the falling and rising edge events with their interrupts enabled. When any CMP 
interrupt happens, the CMP's ISR would turn on the LED light if detecting the output's rising edge, or turn off it when
detecting the output's falling edge.

## Supported Boards
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/driver_examples/cmp/interrupt/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/driver_examples/cmp/interrupt/example_board_readme.md)
- [LPCXpresso55S28](../../../_boards/lpcxpresso55s28/driver_examples/cmp/interrupt/example_board_readme.md)
- [LPCXpresso55S36](../../../_boards/lpcxpresso55s36/driver_examples/cmp/interrupt/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/driver_examples/cmp/interrupt/example_board_readme.md)

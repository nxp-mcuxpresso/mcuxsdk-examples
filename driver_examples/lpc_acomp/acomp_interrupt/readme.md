# acomp_interrupt

## Overview

The ACOMP Interrupt Example shows how to use interrupt with ACOMP driver.

In this example, user should indicate an input channel to capture a voltage signal (can be controlled by user) as the 
ACOMP's negative channel input. On the postive side, the internal voltage ladder is used to generate the fixed voltage about
half value of reference voltage.

When running the project, change the input voltage of user-defined channel, then the comparator's output would change
between logic one and zero when the user-defined channel's voltage crosses the internal ladder's output. The change of
comparator's output would generate the falling and rising edge events with their interrupts enabled. When any ACOMP 
interrupt happens, the ACOMP's ISR would turn on/off the LED light.

## Supported Boards
- [LPC845BREAKOUT](../../../_boards/lpc845breakout/driver_examples/acomp/acomp_interrupt/example_board_readme.md)
- [LPCXpresso802](../../../_boards/lpcxpresso802/driver_examples/acomp/acomp_interrupt/example_board_readme.md)
- [LPCXpresso804](../../../_boards/lpcxpresso804/driver_examples/acomp/acomp_interrupt/example_board_readme.md)
- [LPCXpresso824MAX](../../../_boards/lpcxpresso824max/driver_examples/acomp/acomp_interrupt/example_board_readme.md)
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/acomp/acomp_interrupt/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/acomp/acomp_interrupt/example_board_readme.md)

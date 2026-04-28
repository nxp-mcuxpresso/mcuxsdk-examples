# pit_basic_generic

## Overview
The `pit_basic_generic` project is a simple demonstration program of the SDK PIT driver
(`drivers/dsc_pit`) using a platform-neutral ISR convention. It sets up the PIT
hardware block to trigger a periodic interrupt every 1 second. When the PIT interrupt
is triggered, a message is printed on the UART terminal and an LED is toggled on the
board.

This example uses the same `drivers/dsc_pit` source as the `pit_basic` example,
but provides a platform-neutral main file with a standard C ISR and no DSC-specific
interrupt conventions (no `#pragma interrupt`, no `__CWCC__`, no `DSC_INTERRUPT_*`
variants, no `INTC_SetFastIRQVectorHandler`), so customers on any platform that
reuses this driver see code that is directly relevant to their target.

## Supported Boards

- [FRDM-MCXC353](../../../../examples_int/_boards/frdmmcxc353/driver_examples/pit/basic_generic/example_board_readme.md)

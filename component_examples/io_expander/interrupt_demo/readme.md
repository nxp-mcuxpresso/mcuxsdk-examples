# io_expander_interrupt_demo

## Overview
The io_expander_interrupt_demo application demonstrates per-pin interrupt
handling on an off-chip I/O expander. The demo configures one input pin
with any-edge detection and one output pin; the installed per-pin callback
drives the output pin and the on-board user LED to track the input level —
both go active while the input is asserted, inactive when it is released.

## Supported Boards
- [FRDM-IMXRT1152](../../../_boards/frdmimxrt1152/component_examples/io_expander/interrupt_demo/example_board_readme.md)

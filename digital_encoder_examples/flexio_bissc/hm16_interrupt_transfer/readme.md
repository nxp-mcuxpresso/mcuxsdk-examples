# flexio_bissc_hm16_interrupt_transfer

## Overview
The flexio_bissc_hm16_interrupt_transfer example shows how to use FlexIO as a
BiSS-C master:

In this example, the FlexIO master connects to an HM16 absolute encoder through
the required electrical interface. FlexIO generates the MA clock, receives the
SL response using interrupts, and reports the multi-turn position, single-turn
position, status bits, and CRC result.

## Supported Boards
- [FRDM-MCXA266](../../../_boards/frdmmcxa266/digital_encoder_examples/flexio_bissc/hm16_interrupt_transfer/example_board_readme.md)
- [FRDM-MCXA366](../../../_boards/frdmmcxa366/digital_encoder_examples/flexio_bissc/hm16_interrupt_transfer/example_board_readme.md)

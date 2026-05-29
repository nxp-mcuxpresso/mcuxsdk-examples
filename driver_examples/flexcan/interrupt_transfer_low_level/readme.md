# flexcan_interrupt_transfer_low_level

## Overview
The flexcan_interrupt_transfer_low_level example shows how to use FlexCAN driver in non-blocking interrupt way using low-level hardware APIs:

This example is similar to flexcan_interrupt_transfer but uses low-level hardware APIs instead of the transfer layer:
- Does NOT use `flexcan_handle_t` or `FLEXCAN_TransferCreateHandle()`
- Does NOT use `FLEXCAN_TransferSendNonBlocking()` / `FLEXCAN_TransferReceiveNonBlocking()`
- Uses direct hardware APIs: `FLEXCAN_WriteTxMb()`, `FLEXCAN_ReadRxMb()`, `FLEXCAN_GetMbStatusFlags()`, `FLEXCAN_ClearMbStatusFlags()`
- ISR directly handles interrupts without transfer layer callback mechanism

In this example, two FlexCAN instances on a single board communicate with each other:
- FlexCAN_RX is configured as receiver
- FlexCAN_TX is configured as transmitter
- When user presses any key in the terminal, FlexCAN_TX sends a CAN message to FlexCAN_RX
- FlexCAN_RX receives the message and prints the content to the terminal
- The data byte is incremented for each transmission

This design demonstrates one-way communication between two FlexCAN peripherals on the same board using low-level interrupt-driven APIs.

## Supported Boards
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/flexcan/interrupt_transfer_low_level/example_board_readme.md)

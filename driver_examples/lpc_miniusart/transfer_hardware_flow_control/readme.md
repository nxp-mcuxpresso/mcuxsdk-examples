# usart_transfer_hardware_flow_control

## Overview
The usart_transfer_hardware_flow_control example project demonstrates the usage
of the hardware flow control function. This example sends data to itself(loopback),
and hardware flow control is enabled in the example. The CTS(clear-to-send)
pin is for transmiter to check if receiver is ready, if the CTS pin is asserted,
transmiter starts to send data. The RTS(request-to-send) pin is for receiver
to inform the transmiter if receiver is ready to receive data. So, please connect
RTS to CTS pin directly.

## Supported Boards
- [LPCXpresso845MAX](../../../_boards/lpcxpresso845max/driver_examples/usart/transfer_hardware_flow_control/example_board_readme.md)
- [LPCXpresso860MAX](../../../_boards/lpcxpresso860max/driver_examples/usart/transfer_hardware_flow_control/example_board_readme.md)

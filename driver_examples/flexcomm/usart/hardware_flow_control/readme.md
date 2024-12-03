# usart_hardware_flow_control

## Overview
The usart_hardware_flow_control example project demonstrates the usage
of the hardware flow control function. This example sends data to itself(loopback),
and hardware flow control is enabled in the example. The CTS(clear-to-send)
pin is for transmiter to check if receiver is ready, if the CTS pin is asserted,
transmiter starts to send data. The RTS(request-to-send) pin is for receiver
to inform the transmiter if receiver is ready to receive data. So, please connect
RTS to CTS pin directly.

## Supported Boards
- [LPCXpresso54628](../../../../_boards/lpcxpresso54628/driver_examples/usart/hardware_flow_control/example_board_readme.md)
- [RD-RW612-BGA](../../../../_boards/rdrw612bga/driver_examples/usart/hardware_flow_control/example_board_readme.md)

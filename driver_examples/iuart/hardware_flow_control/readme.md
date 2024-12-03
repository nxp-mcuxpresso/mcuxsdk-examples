# iuart_hardware_flow_control

## Overview
The uart_hardware_flow_control example project demonstrates usage of the hardware
flow control function. This example sends data to itself(loopback), and hardware flow
control is enabled. The CTS(clear-to-send) pin is for transmiter to check if receiver
is ready, if the CTS pin asserts, transmiter start to send data. The RTS(request-to-send)
is a pin for receiver to inform the transmiter if receiver is ready to receive data.
So, please connect RTS to CTS pin directly.

## Supported Boards
- [EVK-MIMX8MM](../../../_boards/evkmimx8mm/driver_examples/uart/hardware_flow_control/example_board_readme.md)

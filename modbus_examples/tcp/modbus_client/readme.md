# modbus_tcp_client

## Overview
This example demonstrates how to use Modbus TCP protocol to control board LED light.

FreeModbus supports bellow feature:
  - Read coil(0x01)
  - Read discrete inputs(0x02)
  - Read holding registers(0x03)
  - Read input registers(0x04)
  - Write single coil(0x05)
  - Write single holding register(0x06)
  - Write board LED status(0x0d)
  - Read board LED status(0x0e)
  - Write multiple coils(0x0f)
  - Write multiple holding registers(0x10)

## Running the demo
If the test passes, serial port output:

  Please input Modbus Server ip address(for example 192.168.1.2):
  Note: don't use backspace key or space key!
  Modbus Server ip: 10.91.20.33

  Initializing PHY...
  [LINK STATE] netif=0, state=down
  [LINK STATE] netif=0, state=up, speed=100M_full

  ************************************************
  Modbus TCP example
  ************************************************
  FOR TASK ETH Get IP address from DHCP...

  IPv4 Address : 10.91.20.66
  IPv4 Subnet mask : 255.255.255.0
  IPv4 Gateway : 10.91.20.254

  Enable LED1
  0 1 0 0 0 6 1 13 0 0 0 1
  Enable LED2
  0 1 0 0 0 6 1 13 0 1 0 1
  ...

Use the MIMXRT1180-EVK board as modbus Server simulator, write board LED status(0x0d) to control LED light

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/modbus_examples/tcp/modbus_client/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/modbus_examples/tcp/modbus_client/example_board_readme.md)

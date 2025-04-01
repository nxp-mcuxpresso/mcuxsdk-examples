# modbus_rtu_client

## Overview
This example demonstrates how to use Modbus RTU protocol to communication.

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

  Modbus RTU Client Example Start!

Use the i.MX RT1180 as modbus Server simulator, write board LED status(0x0d) to control LED light

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/modbus_examples/rtu/modbus_client/example_board_readme.md)
- [IMX943-EVK](../../../_boards/evkmimxrt1180/modbus_examples/rtu/modbus_client/example_board_readme.md)

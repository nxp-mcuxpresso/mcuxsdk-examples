# wwdt_qspi_xip

## Overview
The WWDT Example project is to demonstrate usage of the KSDK wdog driver.
In this example,quick test to show user how to feed watch dog.
User need to feed the watch dog in time before it warning or timeout interrupt.
The WINDOW register determines the highest TV value allowed when a watchdog feed is
performed.
This example runs in QSPI flash with XIP.

## Supported Boards
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/wwdt/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/wwdt/example_board_readme.md)

# co_device_bm

## Overview

The CANopen device demo application demonstrates a CANopen device on the
CANopenNode stack.The application acts as a CANopen device and is managed by
the CANopen manager.

## Running the demo
We need to wait for the CANopen manager example to run before running this example!
When the demo runs, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~
==CANopenNode device example -- Start.==

Allocated 4664 bytes for CANopen objects
CANopenNode - Reset communication...
Current state of the device is <Initializing>!
CANopenNode - Running...
Current state of the device is <PreOperational>!
~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/canopennode_examples/manager/bm/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/canopennode_examples/manager/bm/example_board_readme.md)

# co_manager_bm

## Overview

The CANopen manager demo application demonstrates a CANopen manager on
the CANopenNode stack.The application acts as a CANopen manager and manages
a CANopen device.

## Running the demo
We need to run this example first, and then run the CANopen device example!
When the demo runs, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~
==CANopenNode manager example -- Start.==

Allocated 5400 bytes for CANopen objects
CANopenNode - Reset communication...
CANopenNode - Running...

Please select a test case by pressing the up or down key on your keyboard:
****************************************************
*          |  CANopen Manager  |  CANopen Device   *
****************************************************
* Node-Id  |  0x01             |  0x02             *
* BaudRate |  1000K            |  1000K            *
* State    |  Operational      |  Operational      *
****************************************************
* Sync Message Counter (1 time/s) |   4            *
****************************************************
* SYNC PDO Data     (Per 5 SYNCs) | 0x 7FCCDB5     *
****************************************************
* Event PDO Data    (Test case 4) | 0x 2E70E5C     *
****************************************************
*                    Test cases                    *
****************************************************
* [>] 1. Start CANopen device           (NMT Test) *
* [ ] 2. Set device to pre-operational  (NMT Test) *
* [ ] 3. Read 'Heartbeat producer time' (SDO Test) *
* [ ] 4. Manually obtain the PDO data   (PDO Test) *
****************************************************
*                       Log                        *
****************************************************
The command [1] is executed!
~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/canopennode_examples/manager/bm/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/canopennode_examples/manager/bm/example_board_readme.md)

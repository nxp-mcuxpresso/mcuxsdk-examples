# co_device_basic

## Overview
The co_device_basic example shows CANopen device basic feature on the CANopenNode stack.

CANopen device setup TPDO1(0x1800), TPDO2(0x1801) and RPDO1(0x1400).  
Device TPDO1 will send cyclic frames to manager when it receives SYNC frame with data 0x14(20).  
Press A, trigger device TPDO2 acyclic transmission and send acyclic frames to manager RPDO2.  

Device node ID is 0x04.

## Running the demo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
== CANopenNode device basic EXAMPLE ==
 - Press A Update Manager RPDO (0x1401) mapping parameter (0x2002)

Press any key to start CANopen

Allocated 5344 bytes for CANopen objects
CANopenNode - Reset communication...
CANopenNode - Running...

Press A
Trigger PDO Synchronous Acyclic communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/canopennode_examples/basic/example_board_readme.md)
- [FRDM-MCXE247](../../../_boards/frdmmcxe247/canopennode_examples/basic/example_board_readme.md)
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/canopennode_examples/basic/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/canopennode_examples/basic/example_board_readme.md)

# co_manager_basic

## Overview
The co_manager_basic example shows CANopen manager basic feature on the CANopenNode stack.

CANopen manager setup TPDO1(0x1800), RPDO1(0x1400) and RPDO2(0x1401).  
Press A, manager send NMT command to switch device to Operational mode.  
Press B, manager SDO update device heartbeat producer time to 500ms (0x1017). Default heartbeat producer time is 100ms.  
Press C, manager TPDO1 update device RPDO1 mapping parameter.  
Press D, read manager RPDO1 mapping parameter which is updated by device TPDO1 cyclic transmission.  
Press E, read manager RPDO2 mapping parameter which is updated by device TPDO2 acyclic transmission.  

Manager node ID is 0x03.

## Running the demo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
== CANopenNode manager basic EXAMPLE ==
 - Press A Enter Device Operational mode
 - Press B Update Device Heartbeat producer time to 500ms (0x1017)
 - Press C Update Device RPDO (0x1400) mapping parameter (0x2000)
 - Press D Read Manager RPDO (0x1400) mapping parameter (0x2001)
 - Press E Read Manager RPDO (0x1401) mapping parameter (0x2002)

Press any key to start CANopen and Manager should start first

Allocated 4896 bytes for CANopen objects
CANopenNode - Reset communication...
CANopenNode - Running...

Press A
Device NMT state Operational
Press B
Read back Heartbeat producer time (0x1017) by SDO
SDO Data 0xf4 0x1
Press C
Read back Device RPDO (0x1400) mapping parameter (0x2000) by SDO
SDO Data 0x1 0x0
Press C
Read back Device RPDO (0x1400) mapping parameter (0x2000) by SDO
SDO Data 0x2 0x0
Press C
Read back Device RPDO (0x1400) mapping parameter (0x2000) by SDO
SDO Data 0x3 0x0
Press D
RPDO (0x1400) mapping parameter (0x2001) 0xff
Press E
RPDO (0x1401) mapping parameter (0x2002) 0x1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/canopennode_examples/basic/example_board_readme.md)
- [FRDM-MCXE247](../../../_boards/frdmmcxe247/canopennode_examples/basic/example_board_readme.md)
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/canopennode_examples/basic/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/canopennode_examples/basic/example_board_readme.md)

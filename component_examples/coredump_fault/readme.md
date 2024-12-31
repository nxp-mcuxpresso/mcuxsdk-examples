# coredump_fault

## Overview
The coredump_fault example demonstrates the basic usage of debug/coredump component.

In this example, a Usagefault is intentionally triggered using a division-by-zero operation. 
The fault is then captured in the UsageFault_Handler, where the cause of the fault, the current 
Exception Stack Frame (ESF), and partial RAM contents are saved to flash memory using the coredump component.
 
Afterward, a software reset is triggered. Upon reboot, the fault’s cause is analyzed using the API provided by the coredump component.

Please note that only flash targets are supported!
## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/component_examples/coredump_fault/example_board_readme.md)

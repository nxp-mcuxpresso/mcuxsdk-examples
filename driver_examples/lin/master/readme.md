# lin_master

## Overview
The LIN (Local Interconnect Network) demo application is a use-case of LIN low level driver without
LIN stack. This application pair (master and slave) demonstrates a signal transfer between the
two nodes. Master node and slave node signals are represented by the character array. Master node
repeatedly initializes transfer by generating frame header, master and slave then exchange response
as signal represented by the character array. This demo uses auto-baudrate feature. Both master and
slave node must have implemented the timer that is used by timeout service routine every 500 us and
master node use the same timer to check for switch of the frame table scheduler every 5 ms.
The application and driver use the state machines to handle various of states of the bus on different
levels.

## Running the demo
User can use the button 1 to set master node to sleep and button 2 to wakeup the master node which
is indicated by LED state.

This application supports only unconditional frame types.

Master starts communication on button 2 press. First frame is sent from master to slave. Slave will
update its frame buffer and continue to send its own frame. After that slave will repeate the response
from master and master will send the new frame in the next turn. The next two frames are of diagnostic
type and could be used for customer purposes (for example LIN cluster go to sleep command).
This sequence is than repeated.

If communication is succesfull the master and slave node will both repeatedly print:

LIN DEMO
SLAVE
LIN DEMO
MASTER

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lin/master/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lin/master/example_board_readme.md)
- [FRDM-KW43](../../../_boards/frdmkw43/driver_examples/lin/master/example_board_readme.md)
- [FRDM-MCXW70](../../../_boards/frdmmcxw70/driver_examples/lin/master/example_board_readme.md)

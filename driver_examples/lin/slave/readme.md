# lin_slave

## Overview
The LIN (Local Interconnect Network) demo application is a use-case of LIN low level driver without
LIN stack. This application pair (master and slave) demonstrates a signal transfer between the two
nodes. Master node and slave node signals are represented by the character array. Both master and
slave node must have implemented the timer that is used by timeout service routine every 500 us and
master node use the same timer to check for switch of the frame table scheduler every 5 ms.
The application and driver use the state machines to handle various of states of the bus on different
levels.
This demo uses auto-baudrate feature.
User can use the buttons to sleep and wakeup the master node which is indicated by LED state.
This application supports only unconditional frame types.

## Running the demo
Slave is waiting for master to initialize the communication then exchange a response as signal
represented by the character array. If slave receive a frame header from master it will update its
frame buffer or continue to send its own frame then stay awaiting a new frame header. Master exchanges
the signals four times. The next two frames are of diagnostic type and could be used for customer
purposes (for example LIN cluster go to sleep command). This sequence is than repeated.

If communication is succesfull the master and slave node will both repeatedly print:

LIN DEMO
SLAVE
LIN DEMO
MASTER

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lin/slave/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lin/slave/example_board_readme.md)

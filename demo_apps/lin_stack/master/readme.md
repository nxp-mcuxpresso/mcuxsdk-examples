# lin_stack_master

## Overview
The LIN (Local Interconnect Network) stack demo applications demonstrate signal transfer between two
nodes, master node and slave node. And the lin_stack_master demo is used to demonstrate how to use
the lin stack as master node.
Both master and slave node must have implemented the timer that is used by timeout service routine
every 500 us and master node uses the same timer to check for switch of the frame table scheduler
every 5 ms, but the real period between frames may be larger than 5ms according to the configurations.

- At startup, the master performs a LIN diagnostic READ_BY_IDENTIFIER exchange to identify the
  slave node. It queues the request via the transport layer (`ld_read_by_id`) and the Lin stack
  automatically injects the master-request frame (PID 0x3C) at normal-schedule-table boundaries
  using DIAG_INTERLEAVE_MODE. The master then waits for the slave-response frame (PID 0x3D).
  If the exchange succeeds, the slave's supplier ID, function ID, and variant are printed to the
  console; if no response is received within the timeout the error status is logged and motor
  control starts regardless.
- The master node is in NormalTable schedule table and it uses the LIN frame Motor1State_Cycl
  to receive data from slave node and send selection signal Motor1Selection command to slave
  node by Motor1Control.
- If value from slave is higher than MOTOR1_STOP_VALUE, master will send STOP command to slave node,
  and LED 3 is on.
- If value from slave is higher than MOTOR1_MAX_VALUE, master will send decrease command to slave,
  and LED 1 is on.
- If value from slave is lower than MOTOR_MIN_VALUE, master will send increase command to slave, and
  LED 2 is on.
- The value from slave will keep changing between MOTOR1_MAX_VALUE and MOTOR_MIN_VALUE after reset,
  unless user press slave button 1 to force the value to MOTOR1_STOP_VALUE.
- After master sends stop command, user can press the slave button 2 to set the tick count to 20 to
  restart the loop.
- If the master button 1 was pressed, the master will switch the schedule table to sleep table to
  send go to sleep signal to slave node.
- If the master button 2 was pressed while master in sleep mode, a wake-up signal will be sent by the
  master node to wake up LIN cluster.
- Once running successfully, the state of LEDs on master node and slave node will keep aligned.

## Running the demo
If communication is successful, the master node will print:
~~~~~~~~~~~~
LIN master demo start.
LIN master will receive data from slave node, and send control command to slave.
Diagnostic: reading slave node identity...
Diagnostic: slave identified - Supplier=0x001E Function=0x0001 Variant=0x00
Diagnostic complete. Starting motor control.
 -> LED 2 is ON!
 -> LED 1 is ON!
 -> LED 2 is ON!
 -> LED 1 is ON!
 <press slave button 1>
 -> LED 3 is ON!
 <press slave button 2>
 -> LED 2 is ON!
 -> LED 1 is ON!
........
~~~~~~~~~~~~

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/demo_apps/lin_stack/master/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/demo_apps/lin_stack/master/example_board_readme.md)

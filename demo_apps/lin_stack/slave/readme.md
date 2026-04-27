# lin_stack_slave

## Overview
The LIN (Local Interconnect Network) stack demo applications demonstrate signal transfer between two
nodes, master node and slave node. And the lin_stack_slave demo is used to demonstrate how to use the
lin stack as slave node.
Both master and slave node must have implemented the timer that is used by timeout service routine
every 500 us and master node uses the same timer to check for switch of the frame table scheduler
every 5 ms, but the real period between frames may be larger than 5ms according to the configurations.

- Shortly after the demo starts, the master sends a READ_BY_IDENTIFIER diagnostic request inside
  a master-request frame (PID 0x3C). The slave transport layer processes the request and
  automatically sends the slave's product identity (supplier ID 0x001E, function ID 0x0001,
  variant 0x00) back to the master in a slave-response frame (PID 0x3D). The application
  prints a confirmation line when the request has been handled.
- The master node is in NormalTable schedule table and it uses the LIN frame Motor1State_Cycl
  to receive data from slave node and send selection signal Motor1Selection command to slave
  node by Motor1Control.
- If the command from master is STOP, slave will STOP current data count, and LED 3 is on.
- If the command from master is INCREASE, slave will continue to increase the tick count every 500us,
  and LED 2 is on.
- If the command from master is DECREASE, slave will continue to decrease the tick count every 500us,
  and LED 1 is on.
- The value from slave will keep changing between MOTOR1_MAX_VALUE and MOTOR_MIN_VALUE after reset,
  unless user press slave button 1.
- If the button 1 was pressed, the slave will change the tick count larger than MOTOR1_STOP_VALUE
  and send the data to master node.
- If button 2 was pressed, the slave will set the tick count to 20, and start another loop count.
- Once running successfully, the state of LEDs on master node and slave node will keep aligned.

## Running the demo
If communication is successful, press the SW3 button to increase the ticks count then you will see the LED red is ON.
Then press the SW2 button to reset the ticks count then you will see the LED blue is ON.
And slave node will print:
~~~~~~~~~~~~
LIN slave demo start!
Slave will send data to master and receive command from master.
Slave will control the LED according to the command from master.
Diagnostic: READ_BY_IDENTIFIER request handled.
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
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/demo_apps/lin_stack/slave/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/demo_apps/lin_stack/slave/example_board_readme.md)

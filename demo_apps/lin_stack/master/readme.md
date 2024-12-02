# lin_stack_master

## Overview
The lin_stack_master demo is used to demonstrate how to use the lin stack as master node.
- The master node is in NormalTable schedule table and it uses the LIN frame Motor1State_Cycl
to receive data from slave node and send selection signal Motor1Selection command to slave 
node by Motor1Control.
- If value from slave is higher than MOTOR1_STOP_VALUE, master will send STOP command to slave node.
- If value from slave is higher than MOTOR1_MAX_VALUE, master will send decrease command to slave.
- If value from slave is lower than MOTOR_MIN_VALUE, master will send increase command to slave.
  So, the value from slave will keep changing between MOTOR1_MAX_VALUE and MOTOR_MIN_VALUE.
- If the button 1 was pressed, the master will switch the schedule table to sleep table  to send go
  to sleep signal to slave node.
- If button 2 was pressed while master in sleep mode, a wake up signal will send by master node to wake up  
  LIN cluster.
- Once running successfully, the state of LEDs on master node and slave node will keep aligned.

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/demo_apps/lin_stack/master/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/demo_apps/lin_stack/master/example_board_readme.md)

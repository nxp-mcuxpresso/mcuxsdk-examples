# lin_stack_slave

## Overview
The lin_stack_slave demo is used to demonstrate how to use the lin stack as slave node.
- The master node is in NormalTable schedule table and it uses the LIN frame Motor1State_Cycl
to receive data from slave node and send selection signal Motor1Selection command to slave 
node by Motor1Control.
- If the command from master is STOP, slave will STOP current data count.
- If the command from master is INCREASE, slave will countinue increase the tick count every 500us.
- If the command from master is DECREASE, slave will countinue decrease the tick count every 500us.
  So, the value from slave will keep changing between MOTOR1_MAX_VALUE and MOTOR_MIN_VALUE.
- If the button 1 was pressed, the slave will change the tick count higher than MOTOR1_STOP_VALUE and send 
  the data to master node. 
- If button 2 was pressed, the slave will set the tick count to 0, and start another loop count.
- Once running successfully, the state of LEDs on master node and slave node will keep aligned.

## Supported Boards
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/demo_apps/lin_stack/slave/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/demo_apps/lin_stack/slave/example_board_readme.md)

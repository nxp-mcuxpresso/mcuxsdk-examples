# freertos_lpc_i2c

## Overview
The freertos_i2c example shows an application using RTOS tasks with I2C driver:

Two I2C instances of the single board are used. One i2c instance is used as I2C master and another I2C instance is used as I2C slave.
Two tasks are created. First task is associated with an I2C master operation and second task deals with I2C slave operation.
Example program flow:
    1. I2C master task sends data to I2C slave task.
    2. I2C slave task receive data from I2C master task and send back inverted value of received data.
    3. I2C master task reads data sent back from I2C slave task.
    4. The transmit data and the receive data of both I2C master task and I2C slave task are printed out on terminal.

## Running the demo
When the example runs successfully:
If using 1 board, you can see the similar information from the terminal as below. (Applicable to all boards except TWR-KM34Z75M)



==FreeRTOS I2C example start.==

This example use one i2c instance as master and another as slave on one board.

Master will send data :

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7

0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17

0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f



I2C slave transfer completed successfully.



Slave received data :

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7

0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17

0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f



This time , slave will send data: :

0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8

0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0

0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8

0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0



Master received data :

0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8

0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0

0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8

0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0





End of FreeRTOS I2C example.

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso51U68](../../_boards/lpcxpresso51u68/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/freertos_driver_examples/freertos_lpc_i2c/example_board_readme.md)

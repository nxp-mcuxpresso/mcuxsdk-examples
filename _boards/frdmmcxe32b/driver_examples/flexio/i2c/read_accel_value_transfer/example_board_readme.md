Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
To make this example work, connections needed to be as follows:

    FLEXIO_I2C        connected to  LPI2C1
SCL     J1-4           -->          J7-6
SDA     J1-2           -->          J7-8

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the MCU-Link terminal like:

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value

Found a FXL8974 on board , the device address is 0x18.

The accel values:

status_reg = 0x81 , x =   -50 , y =    11 , z =  2045
status_reg = 0x81 , x =   -50 , y =     9 , z =  2031
status_reg = 0x81 , x =   -49 , y =    12 , z =  2032
status_reg = 0x81 , x =   -50 , y =     6 , z =  2046
status_reg = 0x81 , x =   -51 , y =     8 , z =  2040
status_reg = 0x81 , x =   -45 , y =     4 , z =  2032
status_reg = 0x81 , x =   -52 , y =    13 , z =  2042
status_reg = 0x81 , x =   -53 , y =    12 , z =  2041
status_reg = 0x81 , x =   -51 , y =     8 , z =  2048
status_reg = 0x81 , x =   -52 , y =     9 , z =  2036

End of I2C example .
~~~~~~~~~~~~~~~~~~~~~

Note:
==============
if sensor chip(U32) is not welded on the board, the log would be seen on the MCU-Link terminal like:

FlexIO I2C example read accelerometer value

Not a successful i2c communication

End of I2C example .
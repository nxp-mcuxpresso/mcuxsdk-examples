Hardware requirements
===================
- Mini/micro USB cable
- FRDM-K32L3A6 board
- Personal Computer

Board settings
============
Please ensure there is FXOS8700 on board

Prepare the Demo
===============
1.  Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like:

~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value
Found an FXOS8700 on board, the device address is 0x1E .
The accel values:
status_reg = 0xff , x =   -21 , y =    12 , z =  2007
status_reg = 0xff , x =   -20 , y =    20 , z =  2008
status_reg = 0xff , x =   -19 , y =     4 , z =  2008
status_reg = 0xff , x =   -20 , y =    29 , z =  2022
status_reg = 0xff , x =   -22 , y =    11 , z =  2008
status_reg = 0xff , x =   -18 , y =    33 , z =  2013
status_reg = 0xff , x =   -28 , y =    21 , z =  2005
status_reg = 0xff , x =   -16 , y =    30 , z =  2022
status_reg = 0xff , x =   -29 , y =    13 , z =  2010
status_reg = 0xff , x =   -14 , y =     6 , z =  2024

End of FlexIO I2C example .
~~~~~~~~~~~~~~~~~~~~~

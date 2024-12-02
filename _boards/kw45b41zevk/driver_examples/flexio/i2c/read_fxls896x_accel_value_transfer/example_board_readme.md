Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FlexIO I2C example read accelerometer value
Found a FXLS8967 on board, the device address is 0x18.
The accel values:
x =    30 , y =   -12 , z =  1071
x =    30 , y =   -12 , z =  1071
x =    30 , y =   -12 , z =  1071
x =    19 , y =    18 , z =  1078
x =    19 , y =    18 , z =  1078
x =    41 , y =    -8 , z =  1082
x =    41 , y =    -8 , z =  1082
x =    26 , y =    11 , z =  1086
x =    26 , y =    11 , z =  1086
x =    26 , y =    11 , z =  1086

End of I2C example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

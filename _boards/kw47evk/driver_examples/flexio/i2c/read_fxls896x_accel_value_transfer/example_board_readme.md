Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
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
Found a FXLS8964 on board, the device address is 0x19.
The accel values:
x =    -4 , y =    -4 , z =   983
x =    -4 , y =    -4 , z =   983
x =    11 , y =    38 , z =   995
x =    11 , y =    38 , z =   995
x =    11 , y =    38 , z =   995
x =    26 , y =    11 , z =  1014
x =    26 , y =    11 , z =  1014
x =    15 , y =     7 , z =  1007
x =    15 , y =     7 , z =  1007
x =    15 , y =     7 , z =  1007

End of I2C example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

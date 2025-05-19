Hardware requirements
===================
- Mini/micro USB cable
- MCXW23-EVK Board
- Personal Computer

Board settings
============
Apply the jummpers on J13 1-2, 9-10 and 13-14.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the EVK board J33.
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
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found an FXLS8974 on board, the device address is 0x18.
The accel values:
x =    15 , y =    11 , z =  1119
x =    15 , y =    11 , z =  1119
x =    15 , y =    11 , z =  1119
x =     3 , y =     3 , z =  1077
x =     3 , y =     3 , z =  1077
x =    23 , y =    -8 , z =  1127
x =    23 , y =    -8 , z =  1127
x =    -4 , y =   -12 , z =  1096
x =    -4 , y =   -12 , z =  1096
x =    -4 , y =   -12 , z =  1096

End of I2C example.

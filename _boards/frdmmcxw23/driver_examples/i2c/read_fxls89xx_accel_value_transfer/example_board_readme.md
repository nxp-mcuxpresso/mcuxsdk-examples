Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
Found an FXLS8974 on board, the device address is 0x19.
The accel values:
x =    -4 , y =    19 , z =  1042
x =    -4 , y =    19 , z =  1042
x =    -4 , y =    19 , z =  1042
x =     0 , y =     7 , z =  1029
x =     0 , y =     7 , z =  1029
x =    11 , y =    23 , z =  1046
x =    11 , y =    23 , z =  1046
x =     7 , y =    23 , z =  1022
x =     7 , y =    23 , z =  1022
x =     7 , y =    23 , z =  1022

End of I2C example.
Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

LPI2C example -- Read Accelerometer Value
Found an FXLS8964 on board , the device address is 0x19 . 
The accel values:
x =     3 , y =     0 , z =  1098
x =     3 , y =     0 , z =  1098
x =     0 , y =   -28 , z =  1108
x =     0 , y =   -28 , z =  1108
x =     0 , y =   -28 , z =  1108
x =     0 , y =   -16 , z =  1107
x =     0 , y =   -16 , z =  1107
x =     4 , y =   -32 , z =  1088
x =     4 , y =   -32 , z =  1088
x =    16 , y =   -24 , z =  1091

End of LPI2C example.

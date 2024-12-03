Hardware requirements
===================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a Micro USB cable between the PC host and the OpenSDA USB port on the board.
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
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found an FXOS8700 on board , the device address is 0x1c . 
The accel values:
status_reg = 0xff , x =    63 , y =    82 , z =  2068 
status_reg = 0xff , x =    58 , y =    88 , z =  2079 
status_reg = 0xff , x =    62 , y =    79 , z =  2095 
status_reg = 0xff , x =    64 , y =    91 , z =  2088 
status_reg = 0xff , x =    59 , y =    90 , z =  2089 
status_reg = 0xff , x =    63 , y =    78 , z =  2073 
status_reg = 0xff , x =    57 , y =    89 , z =  2083 
status_reg = 0xff , x =    63 , y =    81 , z =  2091 
status_reg = 0xff , x =    63 , y =    89 , z =  2086 
status_reg = 0xff , x =    59 , y =    87 , z =  2082 

End of I2C example .

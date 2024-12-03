Hardware requirements
===================
- Type-C USB cable
- FRDM-MCXW72 Board
- Personal Computer

Board settings
============
No special is needed.

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
5.  Please note, this example could only run on the FRDM board.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

LPI2C example -- Read Accelerometer Value
Found an FXLS8964 on board , the device address is 0x19 .
The accel values:
x =   122 , y =   761 , z =   376
x =   122 , y =   761 , z =   376
x =   122 , y =   761 , z =   376
x =   125 , y =   680 , z =   470
x =   125 , y =   680 , z =   470
x =   163 , y =   664 , z =   708
x =   163 , y =   664 , z =   708
x =   102 , y =   657 , z =   931
x =   102 , y =   657 , z =   931
x =   102 , y =   657 , z =   931

End of LPI2C example.

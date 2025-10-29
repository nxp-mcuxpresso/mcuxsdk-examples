Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer


Board settings
==============
No special settings are required.


Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
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
LPI2C example -- Read Accelerometer Value
Found an FXLS8974 on board, the device address is 0x18.
The accel values:
x =    18 , y =   -23 , z =  1050
x =    18 , y =   -23 , z =  1050
x =    14 , y =   -23 , z =  1044
x =    14 , y =   -23 , z =  1044
x =    14 , y =   -23 , z =  1044
x =     7 , y =   -12 , z =  1072
x =     7 , y =   -12 , z =  1072
x =    18 , y =   -35 , z =  1044
x =    18 , y =   -35 , z =  1044
x =    18 , y =   -35 , z =  1044

End of LPI2C example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
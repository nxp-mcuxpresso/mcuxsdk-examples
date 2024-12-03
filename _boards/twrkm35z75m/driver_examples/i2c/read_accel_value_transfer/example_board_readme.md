Hardware requirements
=====================
- Micro USB cable
- TWR-KM35Z75M board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
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
When the demo runs successfully, the following message is displayed in the terminal:

I2C example -- Read Accelerometer Value
Found an MMA8451 on board , the device address is 0x1d .
The accel values:
status_reg = 0xff , x =   -10 , y =    31 , z =  2025
status_reg = 0xff , x =    -6 , y =    32 , z =  2022
status_reg = 0xff , x =    -9 , y =    35 , z =  2023
status_reg = 0xff , x =    -4 , y =    32 , z =  2034
status_reg = 0xff , x =   -12 , y =    32 , z =  2020
status_reg = 0xff , x =    -8 , y =    33 , z =  2022
status_reg = 0xff , x =    -8 , y =    33 , z =  2017
status_reg = 0xff , x =    -9 , y =    35 , z =  2024
status_reg = 0xff , x =    -7 , y =    35 , z =  2018
status_reg = 0xff , x =    -5 , y =    33 , z =  2019

End of I2C example .

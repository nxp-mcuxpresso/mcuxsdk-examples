Hardware requirements
===================
- Mini USB cable
- FRDM-KE02Z40M board
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

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.

I2C example -- Read Accelerometer Value
Found an MMA8451 on board , the device address is 0x1d . 
The accel values:
status_reg = 0xff , x =   220 , y =   616 , z =  1946 
status_reg = 0xff , x =   222 , y =   617 , z =  1937 
status_reg = 0xff , x =   217 , y =   615 , z =  1941 
status_reg = 0xff , x =   220 , y =   620 , z =  1941 
status_reg = 0xff , x =   219 , y =   617 , z =  1933 
status_reg = 0xff , x =   221 , y =   614 , z =  1933 
status_reg = 0xff , x =   219 , y =   618 , z =  1933 
status_reg = 0xff , x =   220 , y =   621 , z =  1936 
status_reg = 0xff , x =   221 , y =   616 , z =  1939 
status_reg = 0xff , x =   219 , y =   617 , z =  1945 

End of I2C example .

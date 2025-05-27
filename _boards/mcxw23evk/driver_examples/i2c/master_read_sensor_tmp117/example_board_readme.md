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

i2c master read sensor data example.

Temperature:28.4688

Temperature:28.4766

Temperature:28.4766
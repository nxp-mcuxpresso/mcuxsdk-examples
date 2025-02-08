Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============

Prepare the Demo
================
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
================
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRNG Peripheral Driver Example
Generate 10 random numbers: 
Random[0] = 0x306C5FEC
Random[1] = 0xCB70E82
Random[2] = 0x9427CDBF
Random[3] = 0xC9A727DA
Random[4] = 0x48ABB85B
Random[5] = 0xF55F0CDE
Random[6] = 0xA064B90C
Random[7] = 0xB2D1D7ED
Random[8] = 0xA8720A46
Random[9] = 0x5D9ADC60

 Press any key to continue... 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

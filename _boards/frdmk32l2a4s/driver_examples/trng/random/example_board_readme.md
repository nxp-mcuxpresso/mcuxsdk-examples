Hardware requirements
=====================
- Mini USB cable
- FRDM-K32L2A4S board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRNG Peripheral Driver Example
Generate 10 random numbers:
Random[0] = 0xF84D63F2
Random[1] = 0xC480F1AC
Random[2] = 0x554277AE
Random[3] = 0x13EAFA4E
Random[4] = 0x3FDF87B0
Random[5] = 0x502E3E7A
Random[6] = 0x6B692AAE
Random[7] = 0x8906DB6C
Random[8] = 0x8BC1AC01
Random[9] = 0xD9C74853

 Press any key to continue...
~~~~~~~~~~~~~~~~~~~~~~~~~~~

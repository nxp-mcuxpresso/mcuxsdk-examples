Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC444 board
- Personal Computer

Board settings
============
No special is needed.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  A multimeter may be used to measure the DAC output voltage.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

DAC basic Example.
Please input a value (0 - 4095) to output with DAC: 200
Input value is 200
DAC out: 200

Then user can measure the DAC output pin J6-2 to check responding voltage.


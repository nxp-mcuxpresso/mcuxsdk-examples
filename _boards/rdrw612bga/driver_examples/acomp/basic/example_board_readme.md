Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer

Board settings
==============
- Please remove R78.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ACOMP Basic Example!

Please press any key to get the ACOMP execute result.


The positive input voltage is greater than negative input voltage!


The positive input voltage is less than negative input voltage!


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The analog signal input voltage range is 0-1.8v.Connect the analog signal to HD2-8(GPIO_42),
if input analog signal's voltage greater than 0.9V then serial print "The positive input voltage is greater than negative input voltage!"
otherwise, the serial port print "The positive input voltage is less than negative input voltage!".

Hardware requirements
=====================
- Type-C USB cable
- IMXRTL345MB board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
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
The log below shows the output in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 The LED is taking turns to shine.

 Standard port read: 2018004

 Masked port read: 2010000

 Port state: 18004

 Port state: 18004
 ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press the SW1 will toggle the LED BLUE state.

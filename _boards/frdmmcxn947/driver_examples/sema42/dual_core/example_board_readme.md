Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect a type-c USB cable between the PC host and the MCU-Link USB port (J17) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the demo runs successfully, the log would be seen on the OpenSDA terminal like below
and Red LED will turn on.

~~~~~~~~~~~~~~~~~~~~~
Sema42 example!

Press any key to unlock semaphore and Core 1 will turn off the LED
~~~~~~~~~~~~~~~~~~~~~

When press any key in the terminal, the LED will be turned off and the terminal tells

~~~~~~~~~~~~~~~~~~~~~
Now the LED should be turned off

Sema42 example succeed!
~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860MAX board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J4) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
## Expected Result
------------------------
Power mode switch Demo for LPC8xx.

Selecct an option

    1. Sleep mode

    2. Deep Sleep mode

    3. Power Down mode

    4. Deep power down mode
/* after select power mode, terminal will output */
Select wakeup source

    1. Wkt timer

    2. SW2, wakeup key

    3. SW3, reset key
/* after wakeup, terminal will output */
Wakeup.

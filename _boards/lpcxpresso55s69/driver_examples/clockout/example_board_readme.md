Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S69 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (P6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the clockout demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Clock Output Driver Example.
Please choose one output clock.
        A -- Main Clock.
        B -- PLL0 Clock.
        C -- CLKIN Clock.
        D -- FRO 96 MHz Clock.
        E -- FRO 1 MHz Clock.
        F -- PLLQ Clock.
        G -- Oscillator 32 kHz Clock.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To measure the output clock signal, please use the oscilloscope to probe P17_13(P23_6).


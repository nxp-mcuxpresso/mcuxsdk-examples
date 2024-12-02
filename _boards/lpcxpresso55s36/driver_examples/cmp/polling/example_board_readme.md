Hardware requirements
===================
- Mini/micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
============
(pmux)VREF connect to 1.6V (3.3V * 15 /31).
(nmux)J44-21(PIO1_14).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board.
2.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

CMP polling driver example
RED LED turn on.(pmux > nmux).
RED LED turn off.(pmux < nmux).


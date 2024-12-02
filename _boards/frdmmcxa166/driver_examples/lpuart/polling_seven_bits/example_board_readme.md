Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA166 board
- Personal Computer

Board settings
============
Connect the USB to TTL converter to FRDM-MCXA166 board.
- TX of USB2COM to J5-3(P4_2)
- RX of USB2COM to J5-4(P4_5)
- GND of the USB2COM to J5-8(GND)

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 7 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the log would be seen on the MCU-Link terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example with seven data bits
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
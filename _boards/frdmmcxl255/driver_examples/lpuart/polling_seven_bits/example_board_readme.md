Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
Connect the USB to TTL converter to FRDM-MCXL255 board.
- CM33:
  - TX of USB2COM to SJ1-1(P2_10)
  - RX of USB2COM to SJ2-1(P2_11)
- AON (CM0+):
  - TX of USB2COM to SJ1-3(P0_7)
  - RX of USB2COM to SJ2-3(P0_6)

Make sure you have solder bridges SJ1 and SJ2 set to
- 2-1 for the main core (cm33)
- 2-3 for AON core (cm0+)

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
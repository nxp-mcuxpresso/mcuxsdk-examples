Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- USB to Serial Converter
- Personal Computer

Board settings
============
The connection should be set as following:
- J1-1(PTA16), TX of USB2COM connected
- J1-2(PTA17), RX of USB2COM connected

Prepare the Demo
===============
1. Connect a USB cable between the host PC and the FRDM board J10.
2. Open a serial terminal on PC for USB2COM serial device with these settings:
   - 115200 baud rate
   - 7 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the USB2COM terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example with seven data bits
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

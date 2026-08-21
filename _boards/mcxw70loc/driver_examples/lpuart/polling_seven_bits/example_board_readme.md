Hardware requirements
=====================
- Type-C USB cable
- MCXW70-LOC Board
- Personal Computer
- USB to serial (UART) converter

Board settings
============
Using instance 0 of LPUART interface to transfer data to USB to serial (UART) converter.

LPUART0     CONNECTS TO         USB to serial (UART) converter
Pin Name    Board Location      Pin Name
TXD         JS6-3               RXD
RXD         JS5-3               TXD

Prepare the Demo
===============
1. Connect a USB cable between the host PC and the LOC board J7.
2. Open a serial terminal on PC for USB to serial (UART) converter serial device with these settings:
   - 115200 baud rate
   - 7 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the USB to serial (UART) converter terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Lpuart polling example with seven data bits
Board will send back received characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

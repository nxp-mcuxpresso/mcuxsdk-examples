Hardware requirements
=====================
- Mini USB cable
- LPCXpresso51U68 board
- Personal Computer

Board settings
==============
To make i2c example work, connections needed to be as follows:
        I2C1              connected to     I2C2
SCL     P0_25(J1 pin1)        -->          P0_18(J1 pin11)
SDA     P0_26(J1 pin3)        -->          P0_20(J1 pin13)

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J6) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

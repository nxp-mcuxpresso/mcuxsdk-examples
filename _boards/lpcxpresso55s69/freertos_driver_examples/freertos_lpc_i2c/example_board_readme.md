Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S69 board
- Personal Computer

Board settings
==============
To make i2c example work, connections needed to be as follows:
 	I2C1               connected to           I2C4
SCL     P0_14(P19 pin11)        -->          P1_21(P17 pin1)
SDA    P0_13(P19 pin9)          -->           P1_20(P17 pin3)

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (P6) on the board.
2. Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.


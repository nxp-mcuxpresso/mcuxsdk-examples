Hardware requirements
=====================
- Micro USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
==============
Short JP52 1-2 and JP50 1-2.

To make i2c example work, connections needed to be as follows:
        I2C7              connected to      I2C2
SCL     P1_30(J8-5)        -->          P1_25(J8-4)
SDA     P1_21(J8-6)        -->          P1_24(J8-3)
    
Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (J1) on the board.
2. Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.


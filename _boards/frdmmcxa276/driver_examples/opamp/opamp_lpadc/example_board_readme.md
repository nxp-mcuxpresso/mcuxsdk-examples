Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
Please connect SJ1 to B type (2-3).
Please connect OPAMP1 positive input channel (SJ4 port 3) to a voltage source.
Please connect OPAMP1 positive input channel (SJ4 port 3) to J2-9.
Please connect OPAMP1 negative input channel (SJ5 port 3) to the GND.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for the description of how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One-stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
 Opamp lpadc example.
 Please press any key to get opamp output value.
 opamp0OutputValue = 1644mv
 opamp1PositiveInputValue = 106mv
 Opamp1 output value measured by lpadc: 2703mv
 Opamp1 theoretical output value: 2704mv

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

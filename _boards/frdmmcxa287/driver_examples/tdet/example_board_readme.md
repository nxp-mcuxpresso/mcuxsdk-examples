Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA287 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a type-c USB cable between the PC host and the MCU-Link USB port (J10) on the board
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
This example using TAMPER5 pin as passive tamper.

| TAMPER pin | Function   | PIN NAME | Mikrobus header|
|------------|------------|----------|----------------|
| 5          | Passive    | P5_7     | RST            |

Without connecting tamper pin it should trigger tamper event and output log should look like log below

When Tamper 5 is connected to 3V3, In output log line Line Tampering detected on Passive TAMPER should change to No tampering detected on Passive TAMPER

The log below shows the output of the tdet driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TDET Peripheral Driver Example

Tampering detected Tamper Detect Flag is set

Passive tamper example
Tampering detected on Passive TAMPER

End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


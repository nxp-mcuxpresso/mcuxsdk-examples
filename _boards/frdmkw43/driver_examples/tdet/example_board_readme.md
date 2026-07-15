Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
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
This example using TAMPER0 pin as passive tamper.

| TAMPER pin | Function   | PIN NAME | Arduino header |
|------------|------------|----------|----------------|
| 0          | Passive    | PTD2     | J23 pin 7      |

The following signals are also available on the Arduino headers:
- GND is available on J19 pin 7
- 3V3 MCU is available on J19 pin 8

Without connecting tamper pin it should trigger tamper event and output log should look like log below

When Tamper 0 (PTD2, J23 pin 7) is connected to 3V3 MCU (J19 pin 8), in output log the line Tampering detected on PIN0 should change to No tampering detected on PIN0

The log below shows the output of the tdet driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TDET Peripheral Driver Example

Tampering detected Tamper Detect Flag is set

Passive tamper example
Tampering detected on Passive TAMPER

End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN236 board
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
This example using TAMPER0 pin as passive tamper and TAMPER pin 1 and 4 as active tamper pair.
Pin 1 is set as active tamper rx and Pin 4 as active tamper tx.

| TAMPER pin | Function   | PIN NAME | Mikrobus header|
|------------|------------|----------|----------------|
| 0          | Passive    | P5_2     | RST            |
| 1          | Active RX  | P5_3     | AN             |
| 4          | Active TX  | P5_3     | INT            |

Without connecting tamper pin it should trigger tamper event and output log should look like log below

When Tamper 0 is connected to 3V3, In output log line Line Tampering detected on PIN0 should change to No tampering detected on PIN0

When Tamper 1 and 4 is connected together, In output log line Tampering detected on active tamper should change to No tampering detected on active tamper

The log below shows the output of the tdet driver demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TDET Peripheral Driver Example

Tampering detected Tamper Detect Flag is set

Passive tamper example
Tampering detected on Passive TAMPER

Active tamper example
Tampering detected on active tamper

End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


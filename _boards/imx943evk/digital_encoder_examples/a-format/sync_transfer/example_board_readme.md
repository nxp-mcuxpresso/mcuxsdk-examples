Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK  board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- RS-485 adapter board

Board settings
============
This example requires connecting between FlexIO1 pins and RS-485 adapter board which is connected to the encoder.
- FLEXIO_A_FORMAT_DR_PIN(J48-10) -> D/R pin of RS-485
- FLEXIO_A_FORMAT_TX_PIN(J48-9)  -> RX pin of RS-485
- FLEXIO_A_FORMAT_RX_PIN(J48-11) -> TX pin of RS-485
- 3.3V pin(J49-8)                -> 3.3V pin of RS-485
- GND pin(J49-12)                -> GND pin of RS-485
- 5.0V pin(J49-10) -> 5.0V pin of encoder
- GND pin(J49-14)  -> GND pin of encoder
- A pin of RS-485  -> SD+ pin of encoder
- B pin of RS-485  -> SD- pin of encoder

#### Please note this application can't support running with Linux BSP! ####

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.
Tips: After you insert the USB2COM, a new USBCOM will be found. The function output in new USBCOM.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Encoder A-format example
Encoder ID: 5946590
Multi-turn data: 64474, single-turn data: 147617
[0.10s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 45.000000
[0.20s] Encoder ID: 0x5ABCDE
         Multi-turn data: 64474, single-turn data: 147617
         Temperature: 44.000000
~~~~~~~~~~~~~~~~~~~~~

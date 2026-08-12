Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA366 board
- Personal Computer
- A-format encoder
- RS-485 transceiver

Board settings
============
This example uses FlexIO0 to simulate the A-format master and communicates with the encoder
through an RS-485 transceiver. Connect the encoder to the FRDM-MCXA366 board as below:

The A-format signals are routed to the following FlexIO0 pins (MUX ALT6):
- FLEXIO0_D16 (P2_8,  pin 41) -> DR  (direction / driver-enable)
- FLEXIO0_D17 (P2_9,  pin 42) -> TX  (transmit data to the RS-485 transceiver)
- FLEXIO0_D18 (P2_10, pin 43) -> RX  (receive data from the RS-485 transceiver)

Wire the RS-485 transceiver to the encoder:
- 5.0V -> 5.0V pin of encoder
- GND  -> GND pin of encoder
- SD+  -> SD+ pin of encoder
- SD-  -> SD- pin of encoder

Prepare the Demo
===============
1. Connect a USB Type-C cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
Encoder A-format example
FlexIO Root Clock is 90 MHz

******************** 1 to 1 mode (Only one encoder connects to the board) ********************
****************
* Test case  1 *
****************
> Set the encoder address to 3 ==> successful
****************
* Test case  2 *
****************
> Set the encoder ID to 0x030201 ==> successful
****************
* Test case  3 *
****************
> Get the encoder ID ==> 0x030201 (successful)

......

******************** Running the loop test ********************
[0.10s] Multi-turn data: 64478, single-turn data: 715574
       Temperature: 43.000000

[0.20s] Multi-turn data: 64478, single-turn data: 726567
       Temperature: 43.000000
~~~~~~~~~~~~~~~~~~~~~

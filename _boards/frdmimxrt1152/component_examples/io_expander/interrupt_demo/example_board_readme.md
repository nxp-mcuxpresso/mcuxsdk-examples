Hardware requirements
=====================
- USB-C cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
=============
Default jumpers only — no setting changes are required.

Demo pin map (PCAL6524 I/O expander, both pins are board test points):

| Role              | PCAL6524 pin | Board test point |
|-------------------|--------------|------------------|
| `APP_INPUT_PIN`   | P2_5         | TP12             |
| `APP_OUTPUT_PIN`  | P2_4         | TP9              |

Prepare the Demo
===============
1. Connect a USB-C cable between the PC host and the MCU-Link debug port
   (J13) on the board.
2. Open a serial terminal on the MCU-Link VCOM device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button (SW1) or launch the debugger in your IDE
   to start the demo.

Running the demo
================
On boot the demo prints its banner and waits for interrupts. Pin numbers
match the PCAL6524 port-bit ordering (`APP_INPUT_PIN` = 21, P2_5;
`APP_OUTPUT_PIN` = 20, P2_4):

Bridge TP12 to GND to drive the input LOW. The demo uses any-edge
detection, so the on-board user LED turns **on while TP12 is held LOW**
and **off when TP12 is released** (input active-LOW via the PCAL6524
pull-up). The expander output pin tracks the same level and can be
measured directly at TP9.

~~~~~~~~~~~~~~~~~~~~~~~~

=== I/O Expander Interrupt Demo (Per-Pin Callback) ===

Input pin 21:  pull-up, any-edge interrupt (active-LOW)
Output pin 20: follows input — HIGH while input is held LOW
Waiting for interrupts...

  In pin 21 -> LOW  | Out pin 20 <- HIGH (LED ON)
  In pin 21 -> HIGH | Out pin 20 <- LOW  (LED OFF)
~~~~~~~~~~~~~~~~~~~~~~~~

Hardware requirements
=====================
- USB-C cable
- MIMXRT2660-EVK board
- Jumper wire
- Personal Computer

Board settings
=============
Default jumpers only — no setting changes are required.

Demo pin map (PCAL6524 I/O expander, U23):

| Role              | PCAL6524 pin            | Board net / connector    |
|-------------------|-------------------------|--------------------------|
| `APP_INPUT_PIN`   | P0_0 (WIFI_WAKE_B_3V3)  | J25-2                    |
| `APP_OUTPUT_PIN`  | P2_6 (USB0_PWR)         | R82                      |

The input pin (WIFI_WAKE_B_3V3) is pulled up on board and routed to J25-2. Use
a jumper wire to short J25-2 to any GND point on the board (for example
J34-14) to drive it LOW and trigger an interrupt. Removing the short lets the
on-board pull-up restore the pin to HIGH, which triggers the opposite-edge
interrupt, so the demo loops back and forth. The output pin (USB0_PWR) follows
the input level; verify it by measuring the voltage across R82.



Prepare the Demo
===============
1. Connect a USB-C cable between the PC host and the debug port on the board.
2. Open a serial terminal on the debug VCOM device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button or launch the debugger in your IDE to start
   the demo.

Running the demo
================
On boot the demo prints its banner and waits for interrupts. Pin numbers match
the PCAL6524 port-bit ordering (`APP_INPUT_PIN` = 0, P0_0; `APP_OUTPUT_PIN` =
22, P2_6):

Short J25-2 to a GND point (for example J34-14) with a jumper wire to drive the
input LOW. The demo uses any-edge detection, so the on-board user LED turns
**on while the input is held LOW** and **off when the short is removed** and the
pull-up restores the input to HIGH. Each connect/disconnect produces one
interrupt, so the demo loops back and forth. The expander output pin tracks the
same level and can be verified by measuring the voltage across R82.


~~~~~~~~~~~~~~~~~~~~~~~~

=== I/O Expander Interrupt Demo ===

Input pin 0:  pull-up, any-edge interrupt (active-LOW)
Output pin 22: follows input — HIGH while input is held LOW
Waiting for interrupts...

  In pin  0 -> LOW  | Out pin 22 <- HIGH
  In pin  0 -> HIGH | Out pin 22 <- LOW
~~~~~~~~~~~~~~~~~~~~~~~~


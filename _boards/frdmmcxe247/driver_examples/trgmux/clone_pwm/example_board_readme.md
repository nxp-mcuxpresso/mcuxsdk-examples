Hardware requirements
=====================
- USB-C cable
- FRDM-MCXE247 board
- Personal Computer

Board settings
==============
The example requires connection between the trgmux pin and oscilloscope.
Connect each of the trgmux output pin with the anode of oscilloscope:
J4-12 (TRGMUX_OUT0) -> Anode of oscilloscope.
Or J1-7 (TRGMUX_OUT2) -> Anode of oscilloscope.
Or J4-10 (TRGMUX_OUT3) -> Anode of oscilloscope.
Or J7-1 (TRGMUX_OUT4) -> Anode of oscilloscope.
Or J2-4 (TRGMUX_OUT6) -> Anode of oscilloscope.
Or J7-5 (TRGMUX_OUT7) -> Anode of oscilloscope.
GND  (FRDM-MCXE247 board) -> Cathode of oscilloscope.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the 10KHz PWM on each of the 6 pins
(2 pins are not accessible on FRDM-MCXE247 pin headers).

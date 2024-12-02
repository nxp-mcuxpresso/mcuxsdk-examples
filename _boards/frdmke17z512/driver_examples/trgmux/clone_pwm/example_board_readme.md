Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer

Board settings
==============
The example requires connection between the trgmux pin and oscilloscope.
Connect each of the trgmux output pin with the anode of oscilloscope:
J1-5(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J2-2(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J4-3(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J5-1(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J4-1(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J2-11(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J4-7(FRDM-KE17Z512 board) -> Anode of oscilloscope.
Or J4-5(FRDM-KE17Z512 board) -> Anode of oscilloscope.
GND  (FRDM-KE17Z512 board) -> Cathode of oscilloscope.

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
When the example runs successfully, you can see the 10KHz PWM on each of the 8 pins.

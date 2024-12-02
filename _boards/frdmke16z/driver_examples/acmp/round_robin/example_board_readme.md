Hardware requirements
=====================
- Micro USB cable
- FRDM-KE16Z board
- Personal Computer

Board settings
==============
This example project uses ACMP instance 0 to compare the voltage signal input from channel 0(J4-12)
with the voltage signal(half of VDDA) output by ACMP's internal DAC. In order to observe the low
power wakeup phenomenon, channel 0(J4-12) should be connected to GND signal(J2-14) on the board before
the example running. Example will exit stop mode when channel 0(J4-12) is disconnected from GND signal
and connect to volatage above half of VDDA, the disconnected time is last for about 1s.

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}

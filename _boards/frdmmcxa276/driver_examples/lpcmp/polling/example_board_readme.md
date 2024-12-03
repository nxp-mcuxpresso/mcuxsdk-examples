Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name       Board Location     Pin Name    Board Location
CMP0_IN1/P1_3      J2-17           VDD or GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example project uses LPCMP0 to compare the voltage signal input from CMP0_IN1(P1_3, J2-17)
with the voltage signal(half of VREFI) output by LPCMP's internal DAC.
Connect J2-1(P2_7,VREFI) to J13-1(VDD), this will set the VREFI to VDD(The VREFI is used as LPCMP reference voltage).
Change the IO state of J2-17(P1_3, CMP0_IN1) will toggle the RED LED.

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2. Open a serial terminal on PC for the serial device with these settings:
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
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPCMP Polling Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


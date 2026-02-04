Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC162 board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name       Board Location     Pin Name    Board Location
CMP0_IN1/P1_3      J6-1           VDD or GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example project uses LPCMP0 to compare the voltage signal input from CMP0_IN1(P1_3, J6-1)
with the voltage signal(half of VREFI) output by LPCMP's internal DAC.
Change the IO state of J6-1(P1_3, CMP0_IN1) will toggle the RED LED.

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
LPCMP Interrupt Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


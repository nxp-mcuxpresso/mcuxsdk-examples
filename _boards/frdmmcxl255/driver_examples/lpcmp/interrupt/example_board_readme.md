Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Pin Name            Board Location      Pin Name       Board Location
CM33  CMP0_IN3/P2_15       J6-2 (Mikro bus)  >  VDD or GND      J6_7 or J6_8
CM0+  AON_CMP0_IN5/P0_3    J8_4 (Sensor p.)  >  VDD or GND      J8_2 or J8_1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example project uses LPCMP0 to compare the voltage signal input from CMP0_IN3
with the voltage signal(half of VDD) output by LPCMP's internal DAC.

Make sure you have jumpers SJ1 and SJ2 set to
- 2-1 for the main core (cm33)
- 2-3 for AON core (cm0+)

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


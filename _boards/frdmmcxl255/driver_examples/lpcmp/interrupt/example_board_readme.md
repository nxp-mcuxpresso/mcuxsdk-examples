Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Pin Name            Board Location      Pin Name        Board Location
CM33    CMP0_IN3/P2_15      J6-2 (Mikro bus) >  VDD or GND      J6_7 or J6_8
CM0+    AON_CMP0_IN3/P0_4   J8_21(Sensor p.) >  VDD or GND      J8_2 or J8_1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example project uses LPCMP0 to compare the voltage signal input from input pin
with the voltage signal(half of VDD) output by LPCMP's internal DAC.

Make sure you have solder bridges SJ1 and SJ2 set to
- 2-1 for the main core (cm33)
- 2-3 for AON core (cm0+)

Make sure you have solder bridges SJ28 set to 2-3 for CM0+ core.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
On the main core blue led will shine when input pin is is high.
On AON (cm0+ core) as there is no led, H or L letters will be printed into console instead.

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPCMP Interrupt Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
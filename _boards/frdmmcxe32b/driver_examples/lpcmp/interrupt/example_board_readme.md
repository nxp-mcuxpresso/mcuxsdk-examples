Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
==============
This example project uses LPCMP1 to compare the voltage signal input from channel 0(PTA0)
with the voltage signal output by LPCMP's internal DAC.

The voltage signal input from PTA0 corresponding J9-3 on FRDM-MCXE32B board should be changed
to observe the dynamical IO state. You will see the red led toggle with the voltage change.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
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
The following lines are printed to the serial terminal when the demo program is executed.
The DAC reference comes from 1.2 V PMC bandgap reference in this project, note that the 1.2 V internal reference
voltage is not available in Standby mode.
When the voltage applied on J9-3 in higher than 0.6v, the LED will turn on.
When the voltage applied on J9-3 in lower than 0.6v, the LED will turn off.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPCMP Interrupt Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


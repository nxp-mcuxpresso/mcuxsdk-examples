Hardware requirements
=====================
- Type-C USB cable
- KW47-EVK Board
- Personal Computer

Board settings
==============
This example project uses LPCMP0 to compare the voltage signal input from channel 0(PTA4)
with the voltage signal(half of VREF_OUT) output by LPCMP's internal DAC.
The voltage signal input from PTA4 corresponding J4-5 should
be used to observe the dynamical IO state. You will see the red led toggle with the voltage change.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
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


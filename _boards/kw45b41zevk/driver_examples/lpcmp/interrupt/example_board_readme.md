Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
- Personal Computer

Board settings
==============
This example project uses LPCMP0 to compare the voltage signal input from channel 0(PTA4)
with the voltage signal(half of VREF_OUT) output by LPCMP's internal DAC.
When the demo runs successfully, you will detect voltage toggle on J4.P31 of MB board.
The voltage signal input from PTA4 corresponding J4-29 on mather board should be changed
to observe the dynamical IO state.
If the EVK board is used then the voltage signal input from PTA4 corresponding J4-4 should
be used to observe the dynamical IO state. You will see the green led toggle with the voltage change.

Prepare the Demo
================
1. Connect the micro and mini USB cable between the PC host and the USB ports on the board.
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


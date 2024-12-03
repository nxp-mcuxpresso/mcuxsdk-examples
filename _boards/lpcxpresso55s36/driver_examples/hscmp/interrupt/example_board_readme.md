Hardware requirements
=====================
- Micro USB cable
- Personal Computer
- LPCXpresso55S36 board

Board settings
==============
This example project uses HSCMP to compare the voltage signal input from channel 3(P1_5)
with the voltage signal(half of VREF_OUT) output by HSCMP's internal DAC and output different result.
LED_RED will turn ON/OFF corresponding to different result. So the voltage signal input from P1_28
should be changed to observe the dynamical LED state.
- Input signal to J9-9

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
HSCMP Interrupt Example.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


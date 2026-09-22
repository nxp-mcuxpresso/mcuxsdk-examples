Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXC353 board
- Personal Computer

Board settings
==============
This example project uses PHD to compare the voltage signal input from PHD0_CMP0
with the voltage signal(half of VREF_OUT) output by HSCMP's internal DAC and output different result.
LED_RED will turn ON/OFF corresponding to different result. So the voltage signal input from P2_7
should be changed to observe the dynamical LED state.
- PHD0_CMP0 input signal J4-8(P2_7).

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
PHD Interrupt Example.
The example uses PHD comparator 0 to detect zero-crossing events.
LED turns on when the input rises above the virtual neutral, and turns off when it falls below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


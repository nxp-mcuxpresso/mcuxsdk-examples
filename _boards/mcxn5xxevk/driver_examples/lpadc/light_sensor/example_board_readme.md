Hardware requirements
=====================
- Micro USB cable
- MCX-N5XX-EVK Board
- Personal Computer

Board settings
==============
This example project uses LPADC1 channel 0 to collect the output voltage of the circuit,
which has a light sensor, and the output voltage is proportional to the light illuminance.
When the illuminance increases, the output voltage increases, and the value printed by the
terminal is also larger.

Note that on some MCX-N5XX-EVK boards, the circuit that has a light sensor does not weld up
the resistor SH1 and capacitor C202; before running this example, please check it.

Prepare the Demo
================
1. Connect the micro USB cable between the PC host and the USB ports on the board.
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Light Sensor Example.
LPADC Reference Voltage Config Done!
LPADC Config Done, Full Range: 65536
Please press any key to get the light sensor value.
light sensor value: xxxx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
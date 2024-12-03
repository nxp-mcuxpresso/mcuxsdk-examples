Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW72 Board
- Personal Computer

Board settings
==============
This example project uses LPADC0 channel 6 to collect the output voltage of the circuit,
which has a light sensor, and the output voltage is proportional to the light illuminance.
When the illuminance increases, the output voltage increases, and the value printed by the
terminal is also larger.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Light Sensor Example.
LPADC Config Done, Full Range: 65536
Please press any key to get the light sensor value.
light sensor value: xxxx
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
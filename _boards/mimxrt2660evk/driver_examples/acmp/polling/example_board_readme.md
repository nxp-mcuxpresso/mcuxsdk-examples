Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
This example uses ACMP0 to compare the voltage signal input from INA0 (PIO3_4, J94-17)
with the voltage signal (half of VDDA) output by ACMP's internal DAC.
Please note that the input voltage should be in the range of 0 to 1.8V.
Connect J94-17 to a stable external voltage generator to avoid floating voltage.
The example serial port output may be frequent change otherwise.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
If the input voltage is in the range of 0.9V to 1.8V, the analog input is higher than DAC output.
If the input voltage is in the range of 0V to 0.9V, the analog input is lower than DAC output.

When the demo runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).

The terminal will print CMP's output value when press any key.

Please press any key to get CMP's output value.

The analog input is LOWER than DAC output

The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

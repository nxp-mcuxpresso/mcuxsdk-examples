Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
==============
This example project uses ACMP instance 1 to compare the voltage signal input from External Input6(TP27)
with the voltage signal(half of VDDA) output by ACMP's internal DAC. Terminal will print information
corresponding to different comparison result.

Connect ACMP External Input6(TP27) to stable external voltage generator to avoid floating voltage.
The example serial port output may be frequent change otherwise.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
================
1. Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2. Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, following information can be seen on the terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The example compares analog input to the reference DAC output(CMP positive port).

The terminal will print CMP's output value when press any key.

Please press any key to get CMP's output value.

The analog input is LOWER than DAC output

The analog input is HIGHER than DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

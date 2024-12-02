Hardware requirements
=====================
- Micro USB cable
- FRDM-KE15Z board
- Personal Computer

Board settings
==============
This example project uses ACMP instance 0 to compare the voltage signal input from channel 0(J4-2)
with the voltage signal(half of VDDA) output by ACMP's internal DAC. Green LED will be turned ON/OFF
corresponding to different comparison result.

Connect ACMP channel 0(J4-2) to stable external voltage generator to avoid floating voltage.
The example serial port output may be frequent change otherwise.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}

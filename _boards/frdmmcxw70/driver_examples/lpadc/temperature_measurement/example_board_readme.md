Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW70 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
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
The log below shows the output of the demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPADC Temperature Measurement Example
ADC Full Range: 65536
Please press any key to get temperature from the internal temperature sensor.
Current temperature: 22.319
Current temperature: 22.319
Current temperature: 22.319
Current temperature: 22.319
Current temperature: 22.707
Current temperature: 22.707
Current temperature: 22.707
Current temperature: 22.707
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

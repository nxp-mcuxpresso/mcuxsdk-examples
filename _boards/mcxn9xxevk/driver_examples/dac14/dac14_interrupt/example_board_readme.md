Hardware requirements
=====================
- Mini/micro USB cable
- MCX-N9XX-EVK board
- Personal Computer

Board settings
============
DAC output pin: J19-1

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
5.  A multimeter may be used to measure the DAC output voltage (Connector J19-1).

Running the demo
===============
When the demo runs successfully, the log would be seen on the debug console terminal like:

DAC interrupt Example.
Press any key to trigger the DAC...
DAC14 next output: 500
DAC14 next output: 1000
DAC14 next output: 2000
DAC14 next output: 3000
DAC14 next output: 4000
DAC14 next output: 5000

The user can measure the DAC output pin to check the output voltage.


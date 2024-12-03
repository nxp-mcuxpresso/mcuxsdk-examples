Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-KE02Z40M board
- Personal Computer

Board settings
==============
No special settings are required.
Use oscilloscope to measure output the signal pin at J10-4 pin of board.
Use oscilloscope to measure output the signal pin at J10-6 pin of board.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~
FTM example to output PWM on 2 channels
You will see a change in LED brightness if an LED is connected to the FTM pin
If no LED is connected to the FTM pin, then probe the signal using an oscilloscope
~~~~~~~~~~~~~~~~~~~~~~~

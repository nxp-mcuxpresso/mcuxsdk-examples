Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXE32B board
- Personal Computer
- External signal source

Board settings
============
Connect J3-9 to external PWM signal, can be 20kHz 20% duty cycle PWM signal.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board. 
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
When the example runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~
eMIOS Input Capture Example
Pulse Width count 100

~~~~~~~~~~~~~~~~~~~~~

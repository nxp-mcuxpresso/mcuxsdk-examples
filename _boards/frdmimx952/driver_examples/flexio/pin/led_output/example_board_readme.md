Hardware requirements
=====================
- Micro USB cable
- FRDM-IMX952 board
- J-Link Debug Probe
- 15V~20V USB Type-C PD power supply
- Personal Computer

Board settings
============
Connect J18-5 (GPIO_IO03, FLEXIO1_FLEXIO[3]) and J18-6 (GND) to oscilloscope

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.

Prepare the Demo
===============
1.  Connect 15V~20V USB Type-C PD power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J1 USB Type-C port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
There is no led connected to the pin, need to see oscilloscope, square wave should be observed.
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~
FLEXIO PIN Driver example
The LED is taking turns to shine.
~~~~~~~~~~~~~~~~~~~~~

Hardware requirements
===================
- Mini USB cable
- FRDM-K32L2B board
- Personal Computer
- USB to Com Converter

Board settings
============
Connect pin:
- RX of USB2COM to J1_6
- TX of USB2COM to J2_4
- GND of USB2COM to J2-14

Prepare the Demo
===============
1.  Connect USB to Com Converter between the PC host and the port on the board.
2.  Open a serial terminal on PC for serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Reset the SoC and run the project.

Running the demo
================
You will see the welcome string printed out on the console.
You can send characters to the console back and they will be printed out onto console in a group of 4 characters.

Customization options
=====================


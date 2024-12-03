Overview
========
The TFM CFPA application provides update of CFPA table.
The tfm_cfpa application will download current CFPA table from FFR and it will update the new version number according to current version number.
NOTE: When tfm_cfpa application is loaded to the device and PRINCE is active, the application is executed only ones.
This behaviour is caused by updating of CFPA table in FFR and after reset, the PRINCE input vector is changed in CFPA table and application is invalid.

Toolchain supported
===================
- Keil MDK  5.30

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S16 board
- Personal Computer

Board settings
==============
The jumper setting:
    Default jumpers configuration does not work,  you will need to add JP20 and JP21 (JP22 optional for ADC use)

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
**** The writting to the protected flash region is irreversible operation!!! ****
**** Do you want to continue? (y/n) and press enter:

When y/Y was set:

**** UPDATE CFPA TABLE STARTED ****

Current version: i
Updated version: i+1

**** UPDATE CFPA TABLE FINISHED ****

When n/N was set:

**** UPDATE CFPA TABLE WAS CANCELED ****

When other character was set:

[ERROR] Chosen unsupported character (i)

Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the terminal will display similar information like the following:

ELS PKC asymmetric cipher example

============================
PKC ECC keygen sign verify:pass 
PKC RSA no-verify:pass 
PKC RSA sign no-encode:pass 
PKC RSA-PSS sign SHA256:pass 
PKC RSA-PSS verify SHA256:pass 
PKC ECC Curve25519:pass 
TLS Master session keys:pass 

============================
RESULT: All 7 test PASS!!
ELS example END 


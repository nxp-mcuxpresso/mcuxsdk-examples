Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55s06 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
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
The log below shows the output of the PUF driver example in the terminal window (the key value will vary):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PUF and HACHCRYPT Peripheral Driver Example

PUF Enroll success
PUF Start success

User key:
72 65 68 72 6f 66 6e 6f 6d 6d 6f 63 6e 75 79 72
65 76 73 69 64 72 6f 77 73 73 61 70 73 69 68 54

User key successfully set for HW bus crypto module

User key successfully set on PUF index 1

Successfully reconstructed secret key to HW bus for crypto module

Successfully reconstructed user key:
72 65 68 72 6f 66 6e 6f 6d 6d 6f 63 6e 75 79 72
65 76 73 69 64 72 6f 77 73 73 61 70 73 69 68 54

Setting user key for HASHCRYPT encryption

Encryption success! Printing first 16 bytes:
51 cc 90 bf b5 98 3c cc b8 6f b3 6b 12 55 2d c3

Setting HW bus secret key for HASHCRYPT encryption

Encryption success! Printing first 16 bytes:
51 cc 90 bf b5 98 3c cc b8 6f b3 6b 12 55 2d c3

Success: encrypted outputs are correct


Example end.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

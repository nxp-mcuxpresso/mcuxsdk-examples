Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA287 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs successfully, you can see the similar information from the terminal as below.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SGI PKC PQC example

============================
KEM example using ML-KEM:pass
MlDsa44 keygen, sign and verify:pass
MlDsa44 verify example:pass
MlDsa65 keygen, sign and verify:pass
MlDsa65 verify example:pass
MlDsa87 keygen, sign and verify:pass
MlDsa87 verify example:pass

============================
RESULT: All 7 test PASS!!
SGI PKC example END

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

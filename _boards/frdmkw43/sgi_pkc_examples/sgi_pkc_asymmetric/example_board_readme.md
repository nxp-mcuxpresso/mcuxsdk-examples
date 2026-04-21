Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 board
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
SGI PKC asymmetric cipher example

============================
ECC ECDH key agreement NIST P256:pass
ECC ECDSA NIST P256:pass
RSA key generation:pass
ECC EdDSA verify signature Ed25519:pass
ECC MontDH Curve25519:pass
ECC MontDH Curve448:pass
ECC EdDSA Ed25519:pass

============================
RESULT: All 7 test PASS!!
SGI PKC example END
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

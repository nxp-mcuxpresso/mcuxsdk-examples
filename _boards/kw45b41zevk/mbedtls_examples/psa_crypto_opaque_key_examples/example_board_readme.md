Hardware requirements
=====================
- Mini/micro USB cable
- KW45B41Z-EVK Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J14.
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
When the demo runs successfully, the terminal will display similar information like the following:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Location is c00003

VOLATILE KEYS

ECC keys
224 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA224): PASSED
521 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA512): PASSED
256 bit ECC_KEY_PAIR(SECP_R1) with sign/verify algo ECDSA(SHA256): PASSED
256 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA256): NOT SUPPORTED
384 bit ECC_KEY_PAIR(BRAINPOOL_R1) with sign/verify algo ECDSA(SHA384): NOT SUPPORTED

AES keys
128 bit AES key with encrypt/decrypt algo ECB_NO_PADDING: PASSED
192 bit AES key with encrypt/decrypt algo CBC_NO_PADDING: PASSED
256 bit AES key with encrypt/decrypt algo CTR: PASSED
128 bit AES key with encrypt/decrypt algo CCM: PASSED
256 bit AES key with encrypt/decrypt algo GCM: PASSED
192 bit AES key with sign/verify algo CMAC : PASSED

MAC keys
256 bit HMAC key with sign/verify algo HMAC(SHA-256): PASSED

RSA keys
2048 bit RSA key with sign/verify algo RSA_PKCS1V15(SHA256) : NOT SUPPORTED
2048 bit RSA key with sign/verify algo RSA_PSS(SHA512) : NOT SUPPORTED
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

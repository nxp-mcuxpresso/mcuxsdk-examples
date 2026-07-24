Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Note for RT2660 (pre-silicon): the bundled `ele_fw.h` is the RT1180 ELE
firmware blob. Build is clean, but at runtime the Sentinel subsystem on
RT2660 will not accept this image. Replace `ele_fw.h` with the matching
RT2660 Sentinel firmware blob before exercising any runtime call.

Running the demo
================   
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EdgeLock Enclave secure Sub-System Driver Example:

****************** Load EdgeLock FW ***********************
EdgeLock FW loaded and authenticated successfully.

****************** Generic Crypto AES-CBC ******************
AES-CBC Generic crypto encryption success and output matches expected result.

AES-CBC Generic crypto decryption success and output matches original message.

****************** Generic Crypto AES-CTR ******************
AES-CTR Generic crypto encryption success and output matches expected result.

AES-CTR Generic crypto decryption success and output matches original message.

****************** Generic Crypto AES-OFB ******************
AES-OFB Generic crypto encryption success and output matches expected result.

AES-OFB Generic crypto decryption success and output matches original message.

****************** Generic Crypto AES-GCM ******************
AES-GCM Generic crypto encryption success and output matches expected result.

End of Example with SUCCESS!!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
Example output on terminal:

EdgeLock Enclave secure Sub-System Driver Example:

*********** Load EdgeLock FW ******************************
EdgeLock FW loaded and authenticated successfully.

****************** Start RNG ******************
EdgeLock RNG Start success.
EdgeLock RNG ready to use.

*********** Generic Crypto RSA keygen *********************
RSA Key-Pair generation.
RSA Key-pair generated successfully.

*********** Generic Crypto RSA PKCS#1.5 Signature gen *****
RSA signature with input as digest generated successfully.
RSA signature with input as message generated successfully.

*********** Generic Crypto RSA PSS Signature gen **********
RSA signature generated successfully.

*********** Generic Crypto RSA 2048 PKCS#1.5 Verify *******
RSA verify success

*********** Generic Crypto RSA 3072 PSS Verify ************
RSA verify success

*********** Generic Crypto RSA PKCS#1.5 encrypt ***********
RSA encryption success.

*********** Generic Crypto RSA PKCS#1.5 decrypt ***********
RSA decryption success, plaintext match original one.

*********** Generic Crypto RSA OAEP encrypt ***************
RSA encryption success.

*********** Generic Crypto RSA OAEP decrypt ***************
RSA decryption success, plaintext match original one.

End of Example with SUCCESS!!

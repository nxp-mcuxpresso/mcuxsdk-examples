Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board. 
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
The log below shows the output of the demo in the terminal window:

CAAM Peripheral Driver Example

*CAAM Job Ring 0* :

SHA:done successfully.

HMAC-SHA1:done successfully.

HMAC-SHA224:done successfully.

HMAC-SHA256:done successfully.

HMAC-SHA384:done successfully.

HMAC-SHA512:done successfully.

AES CBC: encrypting using 128 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 192 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 256 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC Blacken: encrypting using 128 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 192 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 256 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

*CAAM Job Ring 1* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

AES GCM Blacken: encrypt done successfully.
AES GCM Blacken: decrypt done successfully.

*CAAM Job Ring 2* :

AES CBC: encrypting using 128 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 192 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC: encrypting using 256 bit key done successfully.
AES CBC: decrypting back done successfully.

AES CBC Blacken: encrypting using 128 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 192 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

AES CBC Blacken: encrypting using 256 bit key done successfully.
AES CBC Blacken: decrypting back done successfully.

*CAAM Job Ring 3* :

AES GCM: encrypt done successfully.
AES GCM: decrypt done successfully.

AES GCM Blacken: encrypt done successfully.
AES GCM Blacken: decrypt done successfully.

AES ECB: encrypt done successfully.
AES ECB: decrypt done successfully.

AES ECB Blacken: encrypt done successfully.
AES ECB Blacken: decrypt done successfully.

AES CCM: encrypt done successfully.
AES CCM: decrypt done successfully.

AES CCM Blacken: encrypt done successfully.
AES CCM Blacken: decrypt done successfully.

AES CTR: encrypt done successfully.
AES CTR: decrypt done successfully.

AES CTR Blacken: encrypt done successfully.
AES CTR Blacken: decrypt done successfully.

RNG : Generate 128-bit random number: 0x6FC2602A8D92A1CB 79B58FEF04B7378
RNG : Random number generated successfully.

CAAM Red Blob Example

Generate Red Blob successfully.

Generate AES Key and blacken it.

Blacken key successfully.

CAAM Black blob Example 

Generate black blob successfully.

CRC: done successfully.

Ecc (private key plain): done successfully.

Ecc (private key encrypted): done successfully.

ECC black key confimation: done successfully.

RSA (plain):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (plain):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (blacken):
Encrypt a secret: done successfully.
Create a signature: done successfully.

RSA (blacken):
Encrypt a secret: done successfully.
Create a signature: done successfully.

Note:
CAAM peripheral works with OCRAM memory. Please avoid using DTCM memory for storage CAAM data.

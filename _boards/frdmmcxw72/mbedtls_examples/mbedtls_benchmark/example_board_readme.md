Hardware requirements
=====================
- Type-C USB cable
- Personal Computer
- FRDM-MCXW72 Board

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J10.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mbedTLS version 2.28.10
fsys=96000000
Using following implementations:
  SHA: ELE200 HW accelerated
  AES: ELE200 HW accelerated ECB, CBC, CCM and CMAC
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: ELE200 HW accelerated ECDSA and ECDH

  SHA-256                  :  5194.16 KiB/s,   18.00 cycles/byte
  SHA-512                  :  4856.55 KiB/s,   19.23 cycles/byte
  AES-CBC-128              :  1555.99 KiB/s,   60.26 cycles/byte
  AES-CBC-192              :  1549.86 KiB/s,   60.51 cycles/byte
  AES-CBC-256              :  1543.00 KiB/s,   60.78 cycles/byte
  AES-GCM-128              :   30.48 KiB/s,  3178.32 cycles/byte
  AES-GCM-192              :   30.33 KiB/s,  3193.40 cycles/byte
  AES-GCM-256              :   30.19 KiB/s,  3208.58 cycles/byte
  AES-CCM-128              :  1269.84 KiB/s,   73.70 cycles/byte
  AES-CCM-192              :  1265.65 KiB/s,   73.94 cycles/byte
  AES-CCM-256              :  1263.35 KiB/s,   74.10 cycles/byte
  AES-CMAC-128             :  1337.71 KiB/s,   69.97 cycles/byte
  AES-CMAC-192             :  1305.24 KiB/s,   71.73 cycles/byte
  AES-CMAC-256             :  1274.36 KiB/s,   73.46 cycles/byte
  AES-CMAC-PRF-128         :  1335.10 KiB/s,   70.13 cycles/byte
  CAMELLIA-CBC-128         :  806.29 KiB/s,  116.27 cycles/byte
  CAMELLIA-CBC-192         :  632.89 KiB/s,  148.21 cycles/byte
  CAMELLIA-CBC-256         :  632.89 KiB/s,  148.21 cycles/byte
  CTR_DRBG (NOPR)          :   30.06 KiB/s,  3222.44 cycles/byte
  CTR_DRBG (PR)            :   23.10 KiB/s,  4235.51 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   38.23 KiB/s,  2516.68 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   33.99 KiB/s,  2841.65 cycles/byte
  RSA-1024                 :    4.33 private/s
  RSA-1024                 :  128.33  public/s
  DHE-2048                 :    0.25 handshake/s
  DH-2048                  :    0.25 handshake/s
  ECDSA-secp521r1          :   23.33 sign/s
  ECDSA-secp384r1          :   45.00 sign/s
  ECDSA-secp256r1          :   76.33 sign/s
  ECDSA-secp521r1          :   12.67 verify/s
  ECDSA-secp384r1          :   29.67 verify/s
  ECDSA-secp256r1          :   71.67 verify/s
  ECDHE-secp521r1          :    1.00 full handshake/s
  ECDHE-secp384r1          :    1.67 full handshake/s
  ECDHE-secp256r1          :    2.67 full handshake/s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Hardware requirements
=====================
- Micro USB cable
- Personal Computer
- KW47-EVK board

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the EVK board J14.
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
mbedTLS version 2.28.8
fsys=96000000
Using following implementations:
  SHA: ELE200 HW accelerated
  AES: ELE200 HW accelerated ECB, CBC, CCM and CMAC
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: ELE200 HW accelerated ECDSA and ECDH

  SHA-256                  :  5124.19 KiB/s,   18.11 cycles/byte
  SHA-512                  :  4799.00 KiB/s,   19.38 cycles/byte
  AES-CBC-128              :  1928.96 KiB/s,   48.49 cycles/byte
  AES-CBC-192              :  1919.80 KiB/s,   48.71 cycles/byte
  AES-CBC-256              :  1910.86 KiB/s,   48.94 cycles/byte
  AES-GCM-128              :   29.83 KiB/s,  3250.47 cycles/byte
  AES-GCM-192              :   29.69 KiB/s,  3266.39 cycles/byte
  AES-GCM-256              :   29.55 KiB/s,  3281.44 cycles/byte
  AES-CCM-128              :  1262.52 KiB/s,   74.13 cycles/byte
  AES-CCM-192              :  1258.31 KiB/s,   74.38 cycles/byte
  AES-CCM-256              :  1256.14 KiB/s,   74.51 cycles/byte
  AES-CMAC-128             :  1292.57 KiB/s,   72.41 cycles/byte
  AES-CMAC-192             :  1262.25 KiB/s,   74.20 cycles/byte
  AES-CMAC-256             :  1233.10 KiB/s,   75.91 cycles/byte
  AES-CMAC-PRF-128         :  1289.19 KiB/s,   72.60 cycles/byte
  CAMELLIA-CBC-128         :  643.85 KiB/s,  145.66 cycles/byte
  CAMELLIA-CBC-192         :  503.25 KiB/s,  186.48 cycles/byte
  CAMELLIA-CBC-256         :  503.25 KiB/s,  186.48 cycles/byte
  CTR_DRBG (NOPR)          :   29.88 KiB/s,  3244.91 cycles/byte
  CTR_DRBG (PR)            :   22.96 KiB/s,  4267.54 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   37.99 KiB/s,  2534.31 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   33.74 KiB/s,  2862.51 cycles/byte
  RSA-1024                 :    3.00 private/s
  RSA-1024                 :   83.67  public/s
  DHE-2048                 :    0.17 handshake/s
  DH-2048                  :    0.15 handshake/s
  ECDSA-secp521r1          :   23.33 sign/s
  ECDSA-secp384r1          :   44.67 sign/s
  ECDSA-secp256r1          :   76.00 sign/s
  ECDSA-secp521r1          :   12.67 verify/s
  ECDSA-secp384r1          :   29.67 verify/s
  ECDSA-secp256r1          :   71.33 verify/s
  ECDHE-secp521r1          :    0.75 full handshake/s
  ECDHE-secp384r1          :    1.00 full handshake/s
  ECDHE-secp256r1          :    2.00 full handshake/s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


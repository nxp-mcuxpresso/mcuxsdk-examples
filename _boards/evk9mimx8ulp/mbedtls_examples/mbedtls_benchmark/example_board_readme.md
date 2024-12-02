Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
**Please note this application can't support running with Linux BSP!**

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW10 to power on the board.
2.  Connect a micro USB cable between the host PC and the J17 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, following information can be seen on the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mbedTLS version 2.27.0
fsys=160000000
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: CASPER HW accelerated ECC256/384/521 and RSA verify

  MD5                      :  4549.71 KB/s,   34.10 cycles/byte
  SHA-1                    :  2500.75 KB/s,   61.73 cycles/byte
  SHA-256                  :  1173.04 KB/s,  133.05 cycles/byte
  SHA-512                  :  835.42 KB/s,  186.99 cycles/byte
  3DES                     :  393.47 KB/s,  397.74 cycles/byte
  DES                      :  997.97 KB/s,  156.46 cycles/byte
  AES-CBC-128              :  1384.84 KB/s,  106.98 cycles/byte
  AES-CBC-192              :  1231.00 KB/s,  121.23 cycles/byte
  AES-CBC-256              :  1106.82 KB/s,  135.63 cycles/byte
  AES-GCM-128              :  443.88 KB/s,  348.61 cycles/byte
  AES-GCM-192              :  406.37 KB/s,  380.12 cycles/byte
  AES-GCM-256              :  379.29 KB/s,  409.43 cycles/byte
  AES-CCM-128              :  317.48 KB/s,  491.08 cycles/byte
  AES-CCM-192              :  281.45 KB/s,  551.98 cycles/byte
  AES-CCM-256              :  255.49 KB/s,  608.67 cycles/byte
  Poly1305                 :  4929.51 KB/s,   31.45 cycles/byte
  CTR_DRBG (NOPR)          :  1144.42 KB/s,  131.18 cycles/byte
  CTR_DRBG (PR)            :  839.12 KB/s,  180.55 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  176.66 KB/s,  888.74 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  163.97 KB/s,  958.29 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :  126.70 KB/s,  1242.79 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :  126.70 KB/s,  1242.79 cycles/byte
  RSA-1024                 :  710.67  public/s
  RSA-1024                 :    4.67 private/s
  DHE-2048                 :    0.29 handshake/s
  DH-2048                  :    0.25 handshake/s
  ECDSA-secp521r1          :    4.67 sign/s
  ECDSA-secp384r1          :    7.33 sign/s
  ECDSA-secp256r1          :   14.67 sign/s
  ECDSA-secp521r1          :    3.67 verify/s
  ECDSA-secp384r1          :    7.00 verify/s
  ECDSA-secp256r1          :   14.00 verify/s
  ECDHE-secp521r1          :    3.00 handshake/s
  ECDHE-secp384r1          :    4.33 handshake/s
  ECDHE-secp256r1          :    8.67 handshake/s
  ECDH-secp521r1           :    5.33 handshake/s
  ECDH-secp384r1           :    8.33 handshake/s
  ECDH-secp256r1           :   17.00 handshake/s
  ECDHE-secp521r1          :    1.67 full handshake/s
  ECDHE-secp384r1          :    2.33 full handshake/s
  ECDHE-secp256r1          :    4.67 full handshake/s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the MCU-Link USB port on the target board. 
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
When the demo runs successfully, the terminal will display similar information like the following (Software only for now):

mbedTLS version 3.5.0
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  6952.79 KB/s,   36.26 cycles/byte
  SHA-1                    :  5056.42 KB/s,   49.96 cycles/byte
  SHA-256                  :  2576.95 KB/s,   98.29 cycles/byte
  SHA-512                  :  1530.60 KB/s,  165.73 cycles/byte
  3DES                     :  726.58 KB/s,  349.59 cycles/byte
  DES                      :  1449.40 KB/s,  174.45 cycles/byte
  3DES-CMAC                :  672.87 KB/s,  377.59 cycles/byte
  AES-CBC-128              :  1976.07 KB/s,  128.29 cycles/byte
  AES-CBC-192              :  1856.94 KB/s,  136.54 cycles/byte
  AES-CBC-256              :  1751.38 KB/s,  144.79 cycles/byte
  AES-XTS-128              :  1388.72 KB/s,  182.70 cycles/byte
  AES-XTS-256              :  1272.29 KB/s,  199.46 cycles/byte
  AES-GCM-128              :  1018.70 KB/s,  249.22 cycles/byte
  AES-GCM-192              :  985.65 KB/s,  257.60 cycles/byte
  AES-GCM-256              :  954.66 KB/s,  265.98 cycles/byte
  AES-CCM-128              :  893.57 KB/s,  284.19 cycles/byte
  AES-CCM-192              :  843.92 KB/s,  300.95 cycles/byte
  AES-CCM-256              :  799.50 KB/s,  317.71 cycles/byte
  AES-CMAC-128             :  1721.66 KB/s,  145.95 cycles/byte
  AES-CMAC-192             :  1642.38 KB/s,  154.43 cycles/byte
  AES-CMAC-256             :  1556.03 KB/s,  163.01 cycles/byte
  AES-CMAC-PRF-128         :  1734.06 KB/s,  146.24 cycles/byte
  Poly1305                 :  11064.74 KB/s,   22.68 cycles/byte
  CTR_DRBG (NOPR)          :  2128.61 KB/s,  119.07 cycles/byte
  CTR_DRBG (PR)            :  1371.74 KB/s,  184.97 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  325.60 KB/s,  781.95 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  299.95 KB/s,  849.06 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :  237.90 KB/s,  1067.48 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :  239.03 KB/s,  1071.52 cycles/byte
  RSA-1024                 :  341.67  public/s
  RSA-1024                 :    9.67 private/s
  DHE-2048                 :    0.75 handshake/s
  DH-2048                  :    1.00 handshake/s
  DHE-3072                 :    0.33 handshake/s
  DH-3072                  :    0.29 handshake/s
  ECDSA-secp521r1          :    8.00 sign/s
  ECDSA-brainpoolP512r1    :    0.67 sign/s
  ECDSA-secp384r1          :   13.00 sign/s
  ECDSA-brainpoolP384r1    :    1.33 sign/s
  ECDSA-secp256r1          :   26.00 sign/s
  ECDSA-secp256k1          :   20.33 sign/s
  ECDSA-brainpoolP256r1    :    3.33 sign/s
  ECDSA-secp224r1          :   34.67 sign/s
  ECDSA-secp224k1          :   23.00 sign/s
  ECDSA-secp192r1          :   47.00 sign/s
  ECDSA-secp192k1          :   30.33 sign/s
  ECDSA-secp521r1          :    3.00 verify/s
  ECDSA-brainpoolP512r1    :    0.29 verify/s
  ECDSA-secp384r1          :    4.67 verify/s
  ECDSA-brainpoolP384r1    :    0.67 verify/s
  ECDSA-secp256r1          :    8.67 verify/s
  ECDSA-secp256k1          :    7.00 verify/s
  ECDSA-brainpoolP256r1    :    1.33 verify/s
  ECDSA-secp224r1          :   11.67 verify/s
  ECDSA-secp224k1          :    8.00 verify/s
  ECDSA-secp192r1          :   16.00 verify/s
  ECDSA-secp192k1          :   10.33 verify/s
  ECDHE-secp521r1          :    2.00 full handshake/s
  ECDHE-brainpoolP512r1    :    0.22 full handshake/s
  ECDHE-secp384r1          :    3.00 full handshake/s
  ECDHE-brainpoolP384r1    :    0.50 full handshake/s
  ECDHE-secp256r1          :    5.00 full handshake/s
  ECDHE-secp256k1          :    4.00 full handshake/s
  ECDHE-brainpoolP256r1    :    0.75 full handshake/s
  ECDHE-secp224r1          :    6.67 full handshake/s
  ECDHE-secp224k1          :    4.67 full handshake/s
  ECDHE-secp192r1          :    9.00 full handshake/s
  ECDHE-secp192k1          :    6.00 full handshake/s
  ECDHE-x25519             :    3.67 full handshake/s
  ECDHE-x448               :    1.67 full handshake/s

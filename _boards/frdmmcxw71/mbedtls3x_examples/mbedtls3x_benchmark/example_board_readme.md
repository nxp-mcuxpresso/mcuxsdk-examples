Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10.
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

mbedTLS version 3.6.0
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  3101.23 KB/s,   30.05 cycles/byte
  SHA-1                    :  1633.50 KB/s,   57.00 cycles/byte
  SHA-256                  :  743.61 KB/s,  125.94 cycles/byte
  SHA-512                  :  525.93 KB/s,  177.94 cycles/byte
  3DES                     :  244.05 KB/s,  385.54 cycles/byte
  DES                      :  631.14 KB/s,  148.60 cycles/byte
  3DES-CMAC                :  225.03 KB/s,  418.13 cycles/byte
  AES-CBC-128              :  932.05 KB/s,  100.51 cycles/byte
  AES-CBC-192              :  806.09 KB/s,  116.27 cycles/byte
  AES-CBC-256              :  710.15 KB/s,  132.02 cycles/byte
  AES-XTS-128              :  820.21 KB/s,  114.26 cycles/byte
  AES-XTS-256              :  641.19 KB/s,  146.26 cycles/byte
  AES-GCM-128              :  214.54 KB/s,  438.55 cycles/byte
  AES-GCM-192              :  207.02 KB/s,  454.55 cycles/byte
  AES-GCM-256              :  200.02 KB/s,  470.56 cycles/byte
  AES-CCM-128              :  393.52 KB/s,  238.37 cycles/byte
  AES-CCM-192              :  347.14 KB/s,  270.39 cycles/byte
  AES-CCM-256              :  310.52 KB/s,  302.41 cycles/byte
  AES-CMAC-128             :  800.52 KB/s,  116.84 cycles/byte
  AES-CMAC-192             :  704.55 KB/s,  132.81 cycles/byte
  AES-CMAC-256             :  628.30 KB/s,  149.03 cycles/byte
  AES-CMAC-PRF-128         :  798.88 KB/s,  117.10 cycles/byte
  Poly1305                 :  3499.05 KB/s,   26.62 cycles/byte
  CTR_DRBG (NOPR)          :  682.02 KB/s,  137.36 cycles/byte
  CTR_DRBG (PR)            :  501.93 KB/s,  186.80 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  112.72 KB/s,  838.80 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  104.67 KB/s,  903.97 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   79.13 KB/s,  1199.14 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   79.13 KB/s,  1199.25 cycles/byte
  RSA-1024                 :   21.00  public/s
  RSA-1024                 :    2.33 private/s
  DHE-2048                 :    0.14 handshake/s
  DH-2048                  :    0.13 handshake/s
  DHE-3072                 :    0.04 handshake/s
  DH-3072                  :    0.04 handshake/s
  ECDSA-secp521r1          :    2.00 sign/s
  ECDSA-brainpoolP512r1    :    0.25 sign/s
  ECDSA-secp384r1          :    3.67 sign/s
  ECDSA-brainpoolP384r1    :    0.50 sign/s
  ECDSA-secp256r1          :    7.33 sign/s
  ECDSA-secp256k1          :    7.00 sign/s
  ECDSA-brainpoolP256r1    :    1.33 sign/s
  ECDSA-secp224r1          :   11.00 sign/s
  ECDSA-secp224k1          :    8.67 sign/s
  ECDSA-secp192r1          :   14.33 sign/s
  ECDSA-secp192k1          :   11.00 sign/s
  ECDSA-secp521r1          :    1.00 verify/s
  ECDSA-brainpoolP512r1    :    0.12 verify/s
  ECDSA-secp384r1          :    1.67 verify/s
  ECDSA-brainpoolP384r1    :    0.25 verify/s
  ECDSA-secp256r1          :    2.67 verify/s
  ECDSA-secp256k1          :    2.67 verify/s
  ECDSA-brainpoolP256r1    :    0.67 verify/s
  ECDSA-secp224r1          :    4.00 verify/s
  ECDSA-secp224k1          :    3.00 verify/s
  ECDSA-secp192r1          :    5.00 verify/s
  ECDSA-secp192k1          :    4.00 verify/s
  ECDHE-secp521r1          :    0.60 full handshake/s
  ECDHE-brainpoolP512r1    :    0.09 full handshake/s
  ECDHE-secp384r1          :    1.00 full handshake/s
  ECDHE-brainpoolP384r1    :    0.20 full handshake/s
  ECDHE-secp256r1          :    1.67 full handshake/s
  ECDHE-secp256k1          :    1.67 full handshake/s
  ECDHE-brainpoolP256r1    :    0.50 full handshake/s
  ECDHE-secp224r1          :    2.33 full handshake/s
  ECDHE-secp224k1          :    2.00 full handshake/s
  ECDHE-secp192r1          :    3.00 full handshake/s
  ECDHE-secp192k1          :    2.33 full handshake/s
  ECDHE-x25519             :    1.67 full handshake/s
  ECDHE-x448               :    0.60 full handshake/s



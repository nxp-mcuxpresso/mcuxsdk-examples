Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
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
When the demo runs successfully, the terminal will display similar information like the following (Software only for now):

mbedTLS version 3.6.0
Using following implementations:
  SHA: Software implementation
  AES: Software implementation
  AES GCM: Software implementation
  DES: Software implementation
  Asymmetric cryptography: Software implementation

  MD5                      :  3104.85 KB/s,   30.03 cycles/byte
  SHA-1                    :  1645.19 KB/s,   56.84 cycles/byte
  SHA-256                  :  744.85 KB/s,  125.86 cycles/byte
  SHA-512                  :  418.49 KB/s,  224.38 cycles/byte
  3DES                     :  242.80 KB/s,  387.54 cycles/byte
  DES                      :  633.26 KB/s,  148.10 cycles/byte
  3DES-CMAC                :  224.43 KB/s,  419.17 cycles/byte
  AES-CBC-128              :  932.63 KB/s,  100.45 cycles/byte
  AES-CBC-192              :  806.52 KB/s,  116.21 cycles/byte
  AES-CBC-256              :  710.48 KB/s,  131.96 cycles/byte
  AES-XTS-128              :  814.85 KB/s,  115.01 cycles/byte
  AES-XTS-256              :  637.92 KB/s,  147.01 cycles/byte
  AES-GCM-128              :  214.48 KB/s,  438.95 cycles/byte
  AES-GCM-192              :  206.98 KB/s,  454.96 cycles/byte
  AES-GCM-256              :  199.99 KB/s,  470.96 cycles/byte
  AES-CCM-128              :  395.23 KB/s,  237.62 cycles/byte
  AES-CCM-192              :  348.47 KB/s,  269.63 cycles/byte
  AES-CCM-256              :  311.63 KB/s,  301.63 cycles/byte
  AES-CMAC-128             :  804.95 KB/s,  116.35 cycles/byte
  AES-CMAC-192             :  708.07 KB/s,  132.33 cycles/byte
  AES-CMAC-256             :  631.06 KB/s,  148.54 cycles/byte
  AES-CMAC-PRF-128         :  803.76 KB/s,  116.54 cycles/byte
  Poly1305                 :  3499.02 KB/s,   26.62 cycles/byte
  CTR_DRBG (NOPR)          :  682.59 KB/s,  137.37 cycles/byte
  CTR_DRBG (PR)            :  502.29 KB/s,  186.84 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  119.89 KB/s,  788.21 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  111.38 KB/s,  848.94 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :   80.27 KB/s,  1182.29 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :   80.27 KB/s,  1182.29 cycles/byte
  RSA-1024                 :   21.33  public/s
  RSA-1024                 :    2.33 private/s
  DHE-2048                 :    0.14 handshake/s
  DH-2048                  :    0.13 handshake/s
  DHE-3072                 :    0.04 handshake/s
  DH-3072                  :    0.04 handshake/s
  ECDSA-secp521r1          :    2.00 sign/s
  ECDSA-brainpoolP512r1    :    0.25 sign/s
  ECDSA-secp384r1          :    3.67 sign/s
  ECDSA-brainpoolP384r1    :    0.50 sign/s
  ECDSA-secp256r1          :    7.67 sign/s
  ECDSA-secp256k1          :    7.33 sign/s
  ECDSA-brainpoolP256r1    :    1.33 sign/s
  ECDSA-secp224r1          :   11.00 sign/s
  ECDSA-secp224k1          :    8.67 sign/s
  ECDSA-secp192r1          :   14.33 sign/s
  ECDSA-secp192k1          :   11.33 sign/s
  ECDSA-secp521r1          :    1.00 verify/s
  ECDSA-brainpoolP512r1    :    0.12 verify/s
  ECDSA-secp384r1          :    1.67 verify/s
  ECDSA-brainpoolP384r1    :    0.25 verify/s
  ECDSA-secp256r1          :    2.67 verify/s
  ECDSA-secp256k1          :    2.67 verify/s
  ECDSA-brainpoolP256r1    :    0.67 verify/s
  ECDSA-secp224r1          :    4.00 verify/s
  ECDSA-secp224k1          :    3.33 verify/s
  ECDSA-secp192r1          :    5.00 verify/s
  ECDSA-secp192k1          :    4.00 verify/s
  ECDHE-secp521r1          :    0.60 full handshake/s
  ECDHE-brainpoolP512r1    :    0.09 full handshake/s
  ECDHE-secp384r1          :    1.00 full handshake/s
  ECDHE-brainpoolP384r1    :    0.20 full handshake/s
  ECDHE-secp256r1          :    1.67 full handshake/s
  ECDHE-secp256k1          :    1.67 full handshake/s
  ECDHE-brainpoolP256r1    :    0.50 full handshake/s
  ECDHE-secp224r1          :    2.67 full handshake/s
  ECDHE-secp224k1          :    2.00 full handshake/s
  ECDHE-secp192r1          :    3.00 full handshake/s
  ECDHE-secp192k1          :    2.67 full handshake/s
  ECDHE-x25519             :    1.67 full handshake/s
  ECDHE-x448               :    0.60 full handshake/s

Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-MCXN236 board
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

  MD5                      :  3467.10 KB/s,   41.94 cycles/byte
  SHA-1                    :  1997.94 KB/s,   73.03 cycles/byte
  SHA-256                  :  1012.04 KB/s,  144.57 cycles/byte
  SHA-512                  :  752.07 KB/s,  194.71 cycles/byte
  3DES                     :  368.63 KB/s,  398.13 cycles/byte
  DES                      :  757.88 KB/s,  193.22 cycles/byte
  3DES-CMAC                :  333.06 KB/s,  440.58 cycles/byte
  AES-CBC-128              :  957.95 KB/s,  152.75 cycles/byte
  AES-CBC-192              :  868.34 KB/s,  168.57 cycles/byte
  AES-CBC-256              :  794.07 KB/s,  184.38 cycles/byte
  AES-XTS-128              :  702.64 KB/s,  208.46 cycles/byte
  AES-XTS-256              :  609.07 KB/s,  240.58 cycles/byte
  AES-GCM-128              :  462.05 KB/s,  317.39 cycles/byte
  AES-GCM-192              :  439.20 KB/s,  333.96 cycles/byte
  AES-GCM-256              :  418.51 KB/s,  350.53 cycles/byte
  AES-CCM-128              :  429.47 KB/s,  341.56 cycles/byte
  AES-CCM-192              :  391.60 KB/s,  374.70 cycles/byte
  AES-CCM-256              :  359.89 KB/s,  407.84 cycles/byte
  AES-CMAC-128             :  857.81 KB/s,  170.65 cycles/byte
  AES-CMAC-192             :  781.81 KB/s,  187.29 cycles/byte
  AES-CMAC-256             :  717.35 KB/s,  204.17 cycles/byte
  AES-CMAC-PRF-128         :  856.47 KB/s,  170.81 cycles/byte
  Poly1305                 :  4032.63 KB/s,   36.01 cycles/byte
  CTR_DRBG (NOPR)          :  921.37 KB/s,  158.84 cycles/byte
  CTR_DRBG (PR)            :  655.38 KB/s,  223.53 cycles/byte
  HMAC_DRBG SHA-1 (NOPR)   :  143.50 KB/s,  1027.51 cycles/byte
  HMAC_DRBG SHA-1 (PR)     :  133.15 KB/s,  1108.08 cycles/byte
  HMAC_DRBG SHA-256 (NOPR) :  106.42 KB/s,  1389.13 cycles/byte
  HMAC_DRBG SHA-256 (PR)   :  106.41 KB/s,  1389.22 cycles/byte
  RSA-1024                 :  118.67  public/s
  RSA-1024                 :    3.67 private/s
  DHE-2048                 :    0.29 handshake/s
  DH-2048                  :    0.29 handshake/s
  DHE-3072                 :    0.09 handshake/s
  DH-3072                  :    0.08 handshake/s
  ECDSA-secp521r1          :    4.00 sign/s
  ECDSA-brainpoolP512r1    :    0.40 sign/s
  ECDSA-secp384r1          :    6.33 sign/s
  ECDSA-brainpoolP384r1    :    1.00 sign/s
  ECDSA-secp256r1          :   13.00 sign/s
  ECDSA-secp256k1          :   12.67 sign/s
  ECDSA-brainpoolP256r1    :    2.33 sign/s
  ECDSA-secp224r1          :   18.67 sign/s
  ECDSA-secp224k1          :   14.33 sign/s
  ECDSA-secp192r1          :   24.33 sign/s
  ECDSA-secp192k1          :   19.00 sign/s
  ECDSA-secp521r1          :    1.67 verify/s
  ECDSA-brainpoolP512r1    :    0.20 verify/s
  ECDSA-secp384r1          :    2.33 verify/s
  ECDSA-brainpoolP384r1    :    0.40 verify/s
  ECDSA-secp256r1          :    4.67 verify/s
  ECDSA-secp256k1          :    4.33 verify/s
  ECDSA-brainpoolP256r1    :    1.00 verify/s
  ECDSA-secp224r1          :    6.67 verify/s
  ECDSA-secp224k1          :    5.33 verify/s
  ECDSA-secp192r1          :    8.33 verify/s
  ECDSA-secp192k1          :    6.67 verify/s
  ECDHE-secp521r1          :    1.00 full handshake/s
  ECDHE-brainpoolP512r1    :    0.15 full handshake/s
  ECDHE-secp384r1          :    1.67 full handshake/s
  ECDHE-brainpoolP384r1    :    0.33 full handshake/s
  ECDHE-secp256r1          :    2.67 full handshake/s
  ECDHE-secp256k1          :    2.67 full handshake/s
  ECDHE-brainpoolP256r1    :    0.60 full handshake/s
  ECDHE-secp224r1          :    4.00 full handshake/s
  ECDHE-secp224k1          :    3.00 full handshake/s
  ECDHE-secp192r1          :    5.00 full handshake/s
  ECDHE-secp192k1          :    4.00 full handshake/s
  ECDHE-x25519             :    2.33 full handshake/s
  ECDHE-x448               :    1.00 full handshake/s


# pn_mbedtls_demo

## Overview
PN intfs firmware provides hardware crypto APIs. Mbedcrypto compatible
functions are created based on these APIs. This project shows how to use
the mbedcrypto compatible functions.

## Running the demo
When the demo runs successfully, the terminal shows the execution result.
Please note that the ECC Key result might be different each time.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PN mbedcrypto example started
AES-ECB-128 ENC: Pass
AES-ECB-256 ENC: Pass
AES-ECB-128 DEC: Pass
AES-ECB-256 DEC: Pass
AES-CBC-128 ENC: Pass
AES-CBC-256 ENC: Pass
AES-CBC-128 DEC: Pass
AES-CBC-256 DEC: Pass
CCM with key size 128-bit: Pass
CCM with key size 256-bit: Pass
GCM with key size 128-bit: Pass
GCM with key size 256-bit: Pass
SHA256: Pass
HMAC using SHA256: Pass
ECC Key Generation
SECP256R1
ECC Private Key:
5B 41 04 7E 92 85 60 55 22 92 6C 43 3B F9 E3 FA
1D 06 57 29 A5 9A 6F 43 08 E1 93 3D BA 23 E0 E6

ECC Public Key X:
DE 9B A4 15 5B 72 A9 A4 73 40 86 A2 D4 83 43 AE
12 DE 82 74 59 0D 68 F8 40 00 64 0A 4F 62 31 F2

ECC Public Key Y:
BD FA BF 57 3F 15 1D AD 00 8B 43 13 2C 9C 07 38
E8 CB 6C F7 56 36 5C 7F 2D C9 A7 A6 CF 68 08 0F

SECP384R1
ECC Private Key:
3B 62 F0 AE 8A 9E E6 2F 68 D3 2A EC DE 1B 6B D2
26 1B 14 20 0F 0B 78 E7 19 42 9C DE B4 DF F8 07
F7 B7 FB 69 FE F8 0A 12 DD EB F5 86 76 BD 3E 50

ECC Public Key X:
36 66 17 EE C3 11 26 0D D2 7E 3A DD 36 32 F1 2B
71 AB 32 09 0D E4 45 35 4C B6 64 20 08 E1 8B D1
FF 67 7E 14 75 49 85 A9 78 04 6F 15 9D 85 0C 40

ECC Public Key Y:
E4 A2 DB 85 4A 41 45 0E 89 0A 92 00 15 96 B3 0D
E2 C9 AE A6 AA F3 1D C3 06 F3 AE C3 BB 68 4B 7D
B9 39 47 D7 F5 D3 AE CC DE 73 C5 4C 5B A3 55 3B

BP256R1
ECC Private Key:
34 F7 F5 B8 60 C4 0A 51 66 0A 22 CC 47 36 83 B1
BD F8 06 C9 AE 35 1B 16 2F AE FB 50 5F 2C BB 53

ECC Public Key X:
2F CB A2 14 88 E4 06 03 69 EA 08 A1 3C 22 8C 57
D5 B6 FC F4 9C C8 A1 74 C7 09 33 50 41 95 23 C5

ECC Public Key Y:
6B 35 E3 A1 A6 2B DB 5B 24 95 99 67 F1 47 17 28
0E D9 A5 25 1D F4 78 62 4C C2 5C 41 A1 9F BB 33

BP384R1
ECC Private Key:
03 48 06 08 02 E5 FB C4 B4 F5 65 71 32 C4 57 34
16 B1 89 CD C9 0C A8 12 A3 D5 3F F6 2D 22 90 5C
9E A3 E5 77 44 EB 44 2B 6B CC 27 09 79 A6 F8 12

ECC Public Key X:
13 1F AC 92 42 43 A4 F8 EF F9 D9 AD 69 A1 F2 0D
C6 6B 97 C6 A2 1E D1 26 BF 1C 7F 74 C5 1A 8B A7
9E F8 04 26 B8 C2 DD CC 7F A5 42 F2 B6 4E C6 3D

ECC Public Key Y:
43 8F D9 A9 03 10 87 D2 53 7C A2 04 92 02 F9 9A
DF 8D 07 59 F4 12 C5 A7 42 45 C6 20 64 5F 4D 24
23 B8 3B 3B 58 A2 0A 91 9E ED 07 4A E7 FB 02 9F

ECDSA sign and verify:
SECP256R1: Pass
SECP384R1: Pass
BP256R1: Pass
BP384R1: Pass
HKDF using SHA256: Pass.
HKDF extract using SHA256: Pass.
HKDF expand using SHA256: Pass.
ECDH using SECP256R1: Pass
ECDH using SECP384R1: Pass
ECDH using BP256R1: Pass
ECDH using BP384R1: Pass
EdDSA Ed25519: Pass.
EdDSA Ed25519ctx: Pass.
EdDSA Ed25519ph: Pass.
Get Asymmetric Public Key From Private Key:
SECP256R1: Pass
SECP384R1: Pass
BP256R1: Pass
BP384R1: Pass

Project success
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

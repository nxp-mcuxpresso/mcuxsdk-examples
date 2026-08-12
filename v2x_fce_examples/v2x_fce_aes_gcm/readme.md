# v2x_fce_aes_gcm

## Overview
The V2X/FCE AES-GCM example is a demonstration program that uses the MCUX SDK
software to perform AES-256 GCM authenticated encryption and decryption through
the EdgeLock V2X Fast Crypto Engine (FCE). It brings up the V2X firmware, opens
a V2X/FCE service, loads AES-256 keys into the FCE key slots, and verifies the
computed ciphertext, authentication tag and decrypted plaintext against a NIST
Known-Answer-Test (KAT) vector.

## Running the demo
Example output on terminal:

*************** EdgeLock V2X/FCE AES-GCM demo **********

****************** Bring up V2X firmware *****************
V2X firmware is running.

****************** Get V2X FW version ********************
V2X FW version: 1.0.7  commit 0x60a0acab

****************** Ping V2X/FCE service ****************
Ping V2X/FCE service successfully.
****************** Open V2X/FCE service ****************
Open V2X/FCE service successfully.

****************** Get V2X/FCE info *******************
V2X/FCE info:
V2X/FCE API version : 1.0.0
Number of FIFO entries : 256

****************** Load AES-256 plain key ****************
Loaded AES-256 key into slot 0.
Loaded AES-256 key into slot 1.
Loaded AES-256 key into slot 2.
Loaded AES-256 key into slot 3.
Loaded AES-256 key into slot 4.
Loaded AES-256 key into slot 5.
Loaded AES-256 key into slot 6.
Loaded AES-256 key into slot 7.

****************** AES-GCM encrypt ***********************
Ciphertext:
8995ae2e6df3dbf96fac7b7137bae67f
Tag:
eca5aa77d51d4a0a14d9c51e1da474ab
Ciphertext and tag match the NIST KAT vector.

****************** AES-GCM decrypt ***********************
Decrypted plaintext:
2d71bcfa914e4ac045b2aa60955fad24
Decrypted plaintext matches the original.

****************** Close V2X/FCE service ***************
Close V2X/FCE service successfully.

*************** EdgeLock V2X/FCE demo END **************

## Supported Boards
- [MIMX943-EVK](../../_boards/imx943evk/v2x_fce/v2x_fce_aes_gcm/example_board_readme.md)

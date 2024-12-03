# puf_hashcrypt_crypto

## Overview
This demo application demonstrates how to use PUF controller which provides a secure key storage
and then sends secret key via dedicated HW bus directly to Hashcrypt, which uses this key to encrypt data.

Usual use of PUF controller consists of these steps:
1. 	Enroll: The controller retrieves the Startup Data (SD) from the memory (SRAM), derives a digital fingerprint, 
	generates the corresponding Activation Code (AC) and sends it to the storage system. 
	Perform this step only once for each device. There is a control register that can block further enrollment. 
	This control register is write only and is reset on a power-on reset.

2. 	Start: The AC generated during the enroll operation and the SD are used to reconstruct the digital fingerprint. 
	It is done after every power-up and reset.

3. 	Generate Key: The controller generates an unique key and combines it with the digital fingerprint to output a key code. 
	Each time a Generate Key operation is executed a new unique key is generated.

4. 	Set Key: The digital fingerprint generated during the Enroll/Start operations and the key provided by the Client Design (CD) 
	are used to generate a Key Code. This KC can be stored externally. Perform this operation only once for each key.

5. 	Get Key: The digital fingerprint generated during the Start operation and the KC generated during a Set Key operation 
	are used to retrieve a stored key. Perform this operation every time a key is needed.

## Supported Boards
- [LPCXpresso55S06](../../../_boards/lpcxpresso55s06/demo_apps/puf_hashcrypt_crypto/example_board_readme.md)
- [LPCXpresso55S16](../../../_boards/lpcxpresso55s16/demo_apps/puf_hashcrypt_crypto/example_board_readme.md)
- [LPCXpresso55S69](../../../_boards/lpcxpresso55s69/demo_apps/puf_hashcrypt_crypto/example_board_readme.md)

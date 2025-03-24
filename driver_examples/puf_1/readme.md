# puf_1

## Overview
The PUF Example project is a demonstration program that uses the KSDK software implement secure key storage using PUF software driver.

Usual use consists of these steps:

1. 	Start: The Activation Code (AC) generated during the enroll operation and the Startup Data (SD) are used to reconstruct the digital fingerprint. 
	It is done after every power-up and reset.

2. 	Generate Key: The controller generates an unique key and combines it with the digital fingerprint to output a key code. 
	Each time a Generate Key operation is executed a new unique key is generated.

3. 	Set Key: The digital fingerprint generated during the Enroll/Start operations and the key provided by the Client Design (CD) 
	are used to generate a Key Code (KC). This KC can be stored externally. Perform this operation only once for each key.

4. 	Get Key: The digital fingerprint generated during the Start operation and the KC generated during a Set Key operation 
	are used to retrieve a stored key. Perform this operation every time a key is needed.

## Supported Boards
- [FRDM-MCXW23](../../_boards/frdmmcxw23/driver_examples/puf/example_board_readme.md)
- [MCXW23-EVK](../../_boards/mcxw23evk/driver_examples/puf/example_board_readme.md)

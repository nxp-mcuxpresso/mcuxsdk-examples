# ele_hseb_demo

## Overview
The ELE HSEB demo project is a demonstration program that uses the
MCUX SDK software to perform crypto operations with HSEB EdgeLock Enclave (ELE)
and usage of its services with direct use of Messaging Unit driver.

## Running the demo
Example output on terminal:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* ELE_HSEB Demo started *

Check if HSEB FW is installed...HSEB FW is installed
Get FW version...Sucess

Check if NVM and RAM key catalogs are formatted...Key catalogs are formatted
Import symmetric keys for cryptographic operation...Success
Try erase keys...Success

Check if NVM and RAM key catalogs are formatted...Key catalogs are not formatted
Formatting key catalogs...Success
Import symmetric keys for cryptographic operation...Success

Exercise crypto operations:
AES crypto - passed
HASH crypto - passed
Session key - passed
Sys authorization - passed
NVM key update - passed
AES get key info - passed
RSA crypto - passed
AEAD crypto - passed

All tests passed!!
Demo end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/ele_hseb/ele_hseb_demo/example_board_readme.md)
- [FRDM-MCXE32B](../../_boards/frdmmcxe32b/ele_hseb/ele_hseb_demo/example_board_readme.md)

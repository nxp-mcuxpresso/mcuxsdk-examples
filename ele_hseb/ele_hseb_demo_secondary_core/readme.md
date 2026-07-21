# ele_hseb_demo_secondary_core

## Overview
This example provides a reference for setting up the default `ele_hseb_demo`
application on the second core on multicore HSE-B-enabled devices.

The application is identical to `ele_hseb_demo` with the difference that it is
executed from the secondary core. The devices, on which this example runs,
are not enabled to run the secondary core independently. The role of the primary
core application is to release the secondary core to run its application.
A code snippet is provided to showcase such a release without the use of
the multicore manager middleware.

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
- [FRDM-MCXE32B](../../_boards/frdmmcxe32b/ele_hseb/ele_hseb_demo/example_board_readme.md)

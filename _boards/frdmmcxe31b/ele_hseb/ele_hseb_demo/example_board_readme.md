Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE31B board
- Personal Computer

Board settings
============

## **ELE HSEB Firmware Installation Guide**

Before using **ELE HSEB**, you must first install the **ELE HSEB Firmware (FW).**

### Firmware Installation Preparation
To install the ELE HSEB FW, the flash must be programmed and a reset must be performed.
  
The most convenient way to program the firmware is by using **J-Link Commander** with the **on-board debugger**.
  
For detailed instructions on setting up the J-Link firmware for the on-board debugger, please refer to the **Getting Started** documentation. A simplified version of the setup steps is provided below:
  - Close jumper **JP3**
  - Use **MCU-Link scripts** (version v3.160 or higher) for J-Link programming
 
### Firmware Location
    <SDK_root>/firmware/edgelock/hseb

### Firmware Installation

#### **J-Link Commands for Firmware Programming**
     Erase 0x400000 0x440000              // For IVT (initial vector table) variant: Erase 0x420000 0x460000
     Loadfile <FW_binary> 0x400000        // Address 0x420000 for IVT
     W8 0x1b000000 0xAABBCCDDDDCCBBAA     // Write special flags to trigger FW installation
     R                                    // Perform reset

#### **(Optional) Verify Flag Write**
    Mem8 0x1b000000 8

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
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
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

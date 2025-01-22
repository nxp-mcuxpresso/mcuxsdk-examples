# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# EdgeLock 2GO Import Blob

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the Secure storage(ITS). The imported objects can then be validated by executing crypto operations.

## Prerequisites

- Any serial communicator
- EdgeLock 2GO secure object blobs stored in the devices flash memory
  - This can be achieved via the [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK)
  - The following default flash location should be used to store the EdgeLock 2GO blobs:
    - **[K4W1]** `0x0C0000`

## Hardware requirements

- K32W148-EVK or KW45B41Z-EVK or KW45B41Z-LOC or FRDM-MCXW71 board
- Micro-USB cable
- Personal Computer

## Board settings

No special settings are required.

## Preparing the application

1.  **[OPTIONAL]** Enable the validation of imported blobs:

    - **[META]** By enabling the Kconfig symbol `VALIDATE_PSA_IMPORT_OPERATION`
    - **[IDE]** By defining `VALIDATE_PSA_IMPORT_OPERATION` as `1` in `mcux_config.h`

    This provides an example of how the imported blobs can be used. Specifically, the example demonstrates:
    - AES-ECB message encryption with a 256 bit key
    - ECDSA SHA-256 message signing

2.  **[OPTIONAL]** Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[K4W1]** Application note [AN14109 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW45) ("Documentation->Secure Files" section).

4.  **[OPTIONAL]** Set the flash location of the EdgeLock 2GO blobs:

    In case you chose a different flash location than the default one mentioned above, you need to change two configuration options.

    - **[META]** The Kconfig symbols `BLOB_AREA` and `BLOB_AREA_SIZE`
    - **[IDE]** The defines `BLOB_AREA` and `BLOB_AREA_SIZE` in `mcux_config.h`

    *ATTENTION: Make sure that your choice does not overlap with any other flash regions.*

5.  Build the application:

    - **[META]** Compile the *el2go_import_blob* project with your desired toolchain.
    - **[IDE]** Compile the *el2go_import_blob* project with your desired toolchain.

6.  Connect the USB-C (K32W148-EVK or KW45B41Z-EVK or KW45B41Z-LOC or FRDM-MCXW71) cable to the PC host and the MCU-Link USB port (J14 [KW45B41Z-EVK or KW45B41Z-LOC]) on the board.

7.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Download the application to the target board:

    - **[META]** `west flash` will download the image. 
    - **[IDE]** Downloading the image.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window.

Importing the blobs from flash into secure-storage (ITS):

```
2 blob(s) imported from flash successfully
```

Validating the blobs (PSA crypto operation with AES master key and ECC key pair):

```
Validate imported blobs

Cipher encrypt passed!

ECC sign passed!
```

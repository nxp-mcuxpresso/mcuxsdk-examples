# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# EdgeLock 2GO Import Blob

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the Secure storage(ITS). The imported objects can then be validated by executing crypto operations.

Max Blob Size:
- **[KW45]**, **[MCX W71]**, **[KW47]**, **[MCX W72]** '1014 or 0x3F6' bytes.

Additional information about EdgeLock 2GO Provisioning for MCUs can be found in AN14624 under the following link https://www.nxp.com/webapp/Download?colCode=AN14624&amp;location=null.

## Definitions

- **[GITHUB]**: refers to the case where the MCU SDK is cloned as GitHub Repository and the EdgeLock 2GO application is build using meta build sytem.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/repo.html.
- **[ZIP]**: refers to the case whete MCU SDK is generated as ZIP package and the EdgeLock 2GO application is imported and build in one of supported IDEs.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/package.html.

## Prerequisites

- Any serial communicator
- EdgeLock 2GO secure object blobs stored in the devices flash memory
  - This can be achieved via the [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK)
  - The following default flash location should be used to store the EdgeLock 2GO blobs:
    - **[KW45]**, **[MCX W71]** , **[KW47]**, **[MCX W72]** `0x0C0000`

## Hardware requirements

- KW45B41Z-EVK or FRDM-MCXW71 or MCXW71-EVK or KW47-EVK or FRDM-MCXW72 or MCXW72-EVK board
- Micro-USB cable
- Personal Computer

## Board settings

- **[KW45]**, **[MCX W71]**, **[KW47]**, **[MCX W72]** The board must be provisioned with an OEM Auth Key Hash.

## Preparing the application

1.  **[OPTIONAL]** Enable the validation of imported blobs:

    - **[GITHUB]** By enabling the Kconfig symbol `VALIDATE_PSA_IMPORT_OPERATION`
    - **[ZIP]** By defining `VALIDATE_PSA_IMPORT_OPERATION` as `1` in `mcux_config.h`

    This provides an example of how the imported blobs can be used. Specifically, the example demonstrates:
    - AES-ECB message encryption with a 256 bit key
    - ECDSA SHA-256 message signing

2.  **[OPTIONAL]** Enable secure boot:

    This feature is optional for **[KW45]** or **[MCX W71]** or **[KW47]** or **[MCX W72]**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[KW45]** Application note [AN14109 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW45) ("Documentation->Public Files" section).
    - **[MCX W71]** Application note [AN14371 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/MCX-W71X) ("Documentation->Public Files" section).
    - **[KW47]**, Application note [AN14371 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW47) ("Documentation->Public Files" section).
    - **[MCX W72]**, Application note [AN14613 Secure Boot using the SEC Tool"](https://www.nxp.com/products/MCX-W72X) ("Documentation->Public Files" section).

4.  **[OPTIONAL]** Set the flash location of the EdgeLock 2GO blobs:

    In case you chose a different flash location than the default one mentioned above, you need to change two configuration options.

    - **[GITHUB]** The Kconfig symbols `BLOB_AREA` and `BLOB_AREA_SIZE`
    - **[ZIP]** The defines `BLOB_AREA` and `BLOB_AREA_SIZE` in `mcux_config.h`

    *ATTENTION: Make sure that your choice does not overlap with any other flash regions.*

5.  Build the application:

    - **[GITHUB]** Compile the *el2go_import_blob* project with your desired toolchain.
    - **[ZIP]** Compile the *el2go_import_blob* project with your desired toolchain.

6.  Connect the PC host to the MCU-Link USB port on the board.

7.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Download the application to the target board:

    - **[GITHUB]** `west flash` will download the image. 
    - **[ZIP]** Downloading the image.

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

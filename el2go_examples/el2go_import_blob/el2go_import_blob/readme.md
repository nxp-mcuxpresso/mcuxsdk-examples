*Copyright 2025 NXP*<br>
*SPDX-License-Identifier: BSD-3-Clause*

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

    - **[GITHUB]** By enabling the following Kconfig symbols
      - `VALIDATE_PSA_IMPORT_MASTER_KEY`
      - `VALIDATE_PSA_IMPORT_KEY_PAIR`
      - `VALIDATE_PSA_IMPORT_CERT`
    - **[ZIP]** By defining following as `1` in `mcux_config.h`
      - `VALIDATE_PSA_IMPORT_MASTER_KEY`
      - `VALIDATE_PSA_IMPORT_KEY_PAIR`
      - `VALIDATE_PSA_IMPORT_CERT`

    This provides an example of how the imported blobs can be used. Specifically, the example demonstrates:
    - AES-ECB message encryption with a 256 bit key
    - ECDSA SHA-256 message signing
    - x509 certificate parsing and extracting public key

    #### Import note:
    - When creating the secure objects on EdgeLock 2GO, select Custom policies.
    - For the AES key, ensure:
        - Permitted algorithm is set to ECB NO PADDING.
        - Device Lifecycle matches the target device's lifecycle.
        - ENCRYPT usage is enabled
    - For the ECC key pair following options should be selected:
        - Device Lifecycle matches the target device's lifecycle.
        - Permitted algorithm should be set to ECDSA SHA 256
        - SIGN MESSAGE usage should be selected
    - For the Certificate, ensure:
        - Device Lifecycle matches the target device's lifecycle.
        - Certificate validation is only possible if key pair validation is also enabled.

2.  Enable secure boot:

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
3 blob(s) imported from flash successfully
```

Validating the blobs (PSA crypto operation with AES master key, ECC key pair and certificate):

```
Validate imported blobs

Cipher encrypt passed!

ECC sign passed!

Certificate in DER format: 3082018B30820132A003020102020708E07700000000300A06082A8648CE3D0403023073310C300A060355040A0C034E5850312A3028060355040B0C21456467654C6F636B32476F52656D6F74655470506C7567416E64547275737443413137303506035504030C2E783530395F696D5F63615F6E31312D303030303030303030303030326130322D30303030303030302D7645323030301E170D3235313131333137333035365A170D3335313131333137333035365A30143112301006035504030C096E313174656D70636E3059301306072A8648CE3D020106082A8648CE3D030107034200042BE9224D4C89CE5CD99CFCEC8E521394DE94B4D2A3F90BC54EE9289B2B86560397482446FD9CAB433A4ED8A5A5DD453F80CAD7A38466A66F2EFC99C950C39AC7A310300E300C0603551D130101FF04023000300A06082A8648CE3D040302034700304402200D2DFE0763A2567BFE11B793AABE5EF313D57ED59035A081881882B156A01AC3022024DAAC7FBFFE400979E6C66B8F17BAD848F72D1C7AAB6579691B0A4F2FC213C4
Subject field in certificate is:CN=test_cn
Issuer field in certificate is:O=NXP, OU=EdgeLock2GoRemoteTpPlugAndTrustCA, CN=x509_IC_-0000000000002a02-00000000-vE200
Public_key: 042BE9224D4C89CE5CD99CFCEC8E521394DE94B4D2A3F90BC54EE9289B2B86560397482446FD9CAB433A4ED8A5A5DD453F80CAD7A38466A66F2EFC99C950C39AC7
```

## Supported Boards
- MCX-W71-EVK
- KW47-EVK
- FRDM-MCXW72
- FRDM-MCXW71
- KW45B41Z-EVK
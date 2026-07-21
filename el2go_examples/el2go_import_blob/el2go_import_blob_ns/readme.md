*Copyright 2025 NXP*<br>
*SPDX-License-Identifier: BSD-3-Clause*

# EdgeLock 2GO Import Blob (NS)

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the ITS storage. The imported objects can then be validated by executing crypto operations.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *el2go_import_blob_s*: Project running in the secure processing environment (S)
- *el2go_import_blob_ns*: Project running in the non-secure processing environment (NS)

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
    - **[RW61X]** `0x084B0000`
	- **[MCX-N5XX-EVK]** `0x001C0000`
	- **[MCX-N9XX-EVK]** `0x001C0000`
	- **[FRDM-MCXN947]** `0x001C0000`
	- **[FRDM-MCXN947T]** `0x001C0000`
	- **[FRDM-MCXN236]** `0x000C4000`

## Hardware requirements

- FRDM-RW612, RD-RW612-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK, FRDM-MCXN947 or FRDM-MCXN236 board
- FRDM-RW612, RD-RW612-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK, FRDM-MCXN947T or FRDM-MCXN236 board
- USB-C (FRDM-RW612, FRDM-MCXN947 or FRDM-MCXN236) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
- USB-C (FRDM-RW612, FRDM-MCXN947T or FRDM-MCXN236) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
- Personal Computer

## Board settings

No special settings are required.

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
        - Device Lifecycle matches the target device’s lifecycle.
        - ENCRYPT usage is enabled
    - For the ECC key pair following options should be selected:
        - Device Lifecycle matches the target device’s lifecycle.
        - Permitted algorithm should be set to ECDSA SHA 256
        - SIGN MESSAGE usage should be selected
    - For the Certificate, ensure:
        - Device Lifecycle matches the target device’s lifecycle.
        - Certificate validation is only possible if key pair validation is also enabled.


2.  Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[RW61X]** Application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).
    - **[MCXN]** Application note [AN14148 "Enabling Secure boot and Trust Provisioning on MCX N series"](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-n-series-microcontrollers/mcx-n94x-54x-highly-integrated-multicore-mcus-with-on-chip-accelerators-intelligent-peripherals-and-advanced-security:MCX-N94X-N54X) ("Documentation->Secure Files" section).

3.  **[OPTIONAL]** Enable support for large blobs:

    In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case you want to support larger blobs (8K is the maximum size supported by PSA), you need to adjust four TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/config_tfm_target.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/config_tfm_target.h)

    ```c
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00
    #define CRYPTO_ENGINE_BUF_SIZE 0x4500
    ```

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)
    ```

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/region_defs.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/region_defs.h)

    ```c
    #define NS_HEAP_SIZE (0x6000)
    ```

4.  **[OPTIONAL]** Set the flash location of the EdgeLock 2GO blobs:

    In case you chose a different flash location than the default one mentioned above, you need to change two configuration options.

    - **[GITHUB]** The Kconfig symbols `BLOB_AREA` and `BLOB_AREA_SIZE`
    - **[ZIP]** The defines `BLOB_AREA` and `BLOB_AREA_SIZE` in `mcux_config.h`

    Also, you need to adjust two TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_EL2GO_NV_DATA_IMPORT_ADDR (0x084A0000)
    #define TFM_EL2GO_NV_DATA_IMPORT_SIZE (0x00060000)
    ```

    *ATTENTION: Make sure that your choice does not overlap with any other flash regions.*

5.  Build the application:

    - **[GITHUB]** Compile the *el2go_import_blob_ns* project with your desired toolchain using `--sysbuild`.
    - **[ZIP]** First compile the *el2go_import_blob_s* project and then the *el2go_import_blob_ns* project.

6.  Connect the USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612], J7 [RD-RW612-BGA], J5 [MCX-N5XX-EVK or MCX-N9XX-EVK] or J17 [FRDM-MCXN947]) on the board.
6.  Connect the USB-C (FRDM-RW612, FRDM-MCXN947T) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612], J7 [RD-RW612-BGA], J5 [MCX-N5XX-EVK or MCX-N9XX-EVK] or J17 [FRDM-MCXN947T]) on the board.

7.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Download the application to the target board:

    - **[GITHUB]** `west flash` will download both the S and the NS image.
    - **[ZIP]** Downloading the S image will also download the NS image.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window.

Booting the S project (TF-M initialization):

```
Booting TF-M v2.1.1
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
[INF][PS] Encryption alg: 0x5500100
```

Jumping to the NS project, importing the blobs from flash into TF-M ITS:

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
- FRDM-RW612
- MCX-N5XX-EVK
- RD-RW612-BGA
- FRDM-MCXN947
- FRDM-MCXN947T
- MCX-N9XX-EVK

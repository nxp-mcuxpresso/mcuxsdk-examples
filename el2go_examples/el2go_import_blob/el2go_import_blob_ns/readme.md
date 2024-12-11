# EdgeLock 2GO Import Blob (NS)

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the ITS storage. The imported objects can then be validated by executing crypto operations.

Workspace structure:
- *el2go_import_blob_s*: Project running in the secure processing environment (S)
- *el2go_import_blob_ns*: Project running in the non-secure processing environment (NS)

## Prerequisites

- Any serial communicator
- EdgeLock 2GO secure object blobs stored in the devices flash memory
  - This can be achieved via the [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK)
  - The following default flash location should be used to store the EdgeLock 2GO blobs:
    - **[RW61X]** `0x084B0000`
    - **[MCXN]** `0x001C0000`

## Hardware requirements

- FRDM-RW612, RD-RW61X-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK or FRDM-MCXN947 board
- USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
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

2.  Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[RW61X]** Application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).
    - **[MCXN]** Application note [AN14148 "Enabling Secure boot and Trust Provisioning on MCX N series"](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-n-series-microcontrollers/mcx-n94x-54x-highly-integrated-multicore-mcus-with-on-chip-accelerators-intelligent-peripherals-and-advanced-security:MCX-N94X-N54X) ("Documentation->Secure Files" section).

3.  **[OPTIONAL]** Enable support for large blobs:

    In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case you want to support larger blobs (8K is the maximum size supported by PSA), you need to adjust two TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/config_tfm_target.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/config_tfm_target.h)

    ```c
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00
    ```

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)
    ```

4.  **[OPTIONAL]** Set the flash location of the EdgeLock 2GO blobs:

    In case you chose a different flash location than the default one mentioned above, you need to change two configuration options.

    - **[META]** The Kconfig symbols `BLOB_AREA` and `BLOB_AREA_SIZE`
    - **[IDE]** The defines `BLOB_AREA` and `BLOB_AREA_SIZE` in `mcux_config.h`

    Also, you need to adjust two TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_EL2GO_NV_DATA_IMPORT_ADDR (0x084A0000)
    #define TFM_EL2GO_NV_DATA_IMPORT_SIZE (0x00060000)
    ```

    *ATTENTION: Make sure that your choice does not overlap with any other flash regions.*

5.  Build the application:

    - **[META]** Compile the *el2go_import_blob_ns* project with your desired toolchain using `--sysbuild`.
    - **[IDE]** First compile the *el2go_import_blob_s* project and then the *el2go_import_blob_ns* project.

6.  Connect the USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612], J7 [RD-RW61X-BGA], J5 [MCX-N5XX-EVK or MCX-N9XX-EVK] or J17 [FRDM-MCXN947]) on the board.

7.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Download the application to the target board:

    - **[META]** `west flash` will download both the S and the NS image. 
    - **[IDE]** Downloading the S image will also download the NS image.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window.

Booting the S project (TF-M initialization):

```
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.8.0
```

Jumping to the NS project, importing the blobs from flash into TF-M ITS:

```
2 blob(s) imported from flash successfully
```

Validating the blobs (PSA crypto operation with AES master key and ECC key pair):

```
Validate imported blobs

Cipher encrypt passed!

ECC sign passed!
```

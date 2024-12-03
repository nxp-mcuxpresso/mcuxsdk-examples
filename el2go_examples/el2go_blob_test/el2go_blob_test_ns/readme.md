# EdgeLock 2GO Blob Test (NS)

This is a test suite which imports and validates EL2GO blobs and their usage with PSA.

Workspace structure:
- *el2go_blob_test_s*: Project running in the secure processing environment (S)
- *el2go_blob_test_ns*: Project running in the non-secure processing environment (NS)

## Prerequisites

- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- Any serial communicator
- Python >= 3.9 with packages from [requirements.txt](../../../../middleware/nxp_iot_agent/tst/el2go_blob_test/scripts/requirements.txt)
- **[MCXN]** [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK) or standalone Provisioning Firmware

*NOTE: The Python scripts refered to in this example can be found in the [scripts](../../../../middleware/nxp_iot_agent/tst/el2go_blob_test/scripts) folder in the example directory.*

## Hardware requirements

- FRDM-RW612, RD-RW61X-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK or FRDM-MCXN947 board
- USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
- Personal Computer

## Board settings

- **[RW61X]** The board must be provisioned with an OEM Auth Key Hash.
- **[MCXN]** The board must be provisioned with an OEM Auth Key Hash and an OEM FW Decryption Key in OEM OPEN lifecycle.

## Preparing the application

1.  Create device specific blobs:

    - Inline mode:
        1. Obtain a RTP JSON file from EdgeLock 2GO containing the desired blobs for your board
        2. **[MCXN]** Connect your board and rewrap all the blobs in the RTP JSON file:
            ```sh
            el2go_blob_rewrap.py [RAW_RTP_JSON_PATH] [PROVISIONING_FIRMWARE_PATH] [COM_PORT] [RTP_JSON_PATH]
            ```
        2. Run the file trough the preprocessor:
            ```sh
            el2go_blob_test_pre.py [RTP_JSON_PATH]
            ```
    - **[MCXN]** Memory mode:
        1. Create a device group in EdgeLock 2GO containing the desired blobs for your board
        2. Provision the blobs to your board via the `el2go-host` app from SPSDK (set the `additional_data_address` property of the config file to `0x001C0000`):
            ```sh
            el2go-host provision-device -p [COM_PORT] --config [CONFIG_PATH] --workspace [WORKSPACE_PATH]
            ```
        3. Run the RTP JSON file downloaded by `el2go-host` trough the preprocessor, specifying the memory location method and address:
            ```sh
            el2go_blob_test_pre.py [WORKSPACE_PATH]/provisionings.json --storage_mode memory --blob_address 0x001C0000
            ```
        *NOTE: This method only works for a maximum of 16 blobs, not exceeding 16KB in total.*

    *ATTENTION: Make sure the lifecycle and OEM Auth Key Hash of your blobs match the one provisioned to the board. Attempting to rewrap/provision blobs with an OEM CLOSED lifecycle to an OEM OPEN board will change the lifecycle!*

2.  **[OPTIONAL]** Enable all possible variations:

    By default, the test suite only runs variations that are expected to pass for a given board. If you want to run all possible tests instead, you can specify that:

    - **[META]** By disabling the Kconfig symbol `RUN_VERIFIED_ONLY`
    - **[IDE]** By defining `RUN_VERIFIED_ONLY` as `0` in `mcux_config.h`

    *Note: If you input an entirely unsupported blob, the testcase will still run and fail, even if `RUN_VERIFIED_ONLY` is enabled.*

3.  Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[RW61X]** Application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).
    - **[MCXN]** Application note [AN14148 "Enabling Secure boot and Trust Provisioning on MCX N series"](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-n-series-microcontrollers/mcx-n94x-54x-highly-integrated-multicore-mcus-with-on-chip-accelerators-intelligent-peripherals-and-advanced-security:MCX-N94X-N54X) ("Documentation->Secure Files" section).

4.  **[OPTIONAL]** Enable support for large blobs:

    In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case you want to support larger blobs (8K is the maximum size supported by PSA), you need to adjust two TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/config_tfm_target.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/config_tfm_target.h)

    ```c
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00
    ```

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)
    ```

    After that, you can enable the testing of large blobs:

    - **[META]** By enabling the Kconfig symbol `LARGE_BLOBS_ENABLED`
    - **[IDE]** By defining `LARGE_BLOBS_ENABLED` as `1` in `mcux_config.h`

5.  Build the application:

    - **[META]** Compile the *el2go_blob_test_ns* project with your desired toolchain using `--sysbuild`.
    - **[IDE]** First compile the *el2go_blob_test_s* project and then the *el2go_blob_test_ns* project.

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

10. **[OPTIONAL]** Convert results to JUnit:

    If you capure the console output of the application, you can feed it into the postprocessor to recieve the results in the JUnit format:

    ```sh
    el2go_blob_test_post.py [CONSOLE_OUTPUT_PATH] [JUNIT_OUT_PATH]
    ```


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

Jumping to the NS project, running the blob test suite:

```
#### Start EL2GO blob tests ####
Running test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX)
> Executing test EL2GO_BLOB_TEST_GENERIC_0001 
  Description: 'External CERTIFICATE 425B EXPORT NONE'
  Test EL2GO_BLOB_TEST_GENERIC_0001 - PASSED (16 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_0002 
  Description: 'External KEYPAIR BRAINPOOLP384R1 SIGVERMSG ECDSAANYHASH'
  > Executing variation SHA_224
    Variation SHA_224 - PASSED (126 ms)
  > Executing variation SHA_512_224
    Variation SHA_512_224 - PASSED (124 ms)
  > Executing variation SHA_256
    Variation SHA_256 - PASSED (126 ms)
  > Executing variation SHA_512_256
    Variation SHA_512_256 - PASSED (126 ms)
  > Executing variation SHA_384
    Variation SHA_384 - PASSED (125 ms)
  > Executing variation SHA_512
    Variation SHA_512 - PASSED (125 ms)
  Test EL2GO_BLOB_TEST_GENERIC_0002 - PASSED (814 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_0003 
  Description: 'External KEYPAIR ECCMONTDH25519 DERIVE ECDH'
  Test EL2GO_BLOB_TEST_GENERIC_0003 - PASSED (52 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_0004 
  Description: 'Internal KEYPAIR NISTP256 SIGHASH ECDSASHA256'
  Test EL2GO_BLOB_TEST_GENERIC_0004 - PASSED (21 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_0005 
  Description: 'Internal KEYPAIR NISTP256 SIGHASH ECDSASHA256'
  Test EL2GO_BLOB_TEST_GENERIC_0005 - PASSED (19 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_0006 
  Description: 'External CERTIFICATE 420B EXPORT NONE'
  psa_import_key returned -149
  Failed at psa_blob_export_test:1526
  Test EL2GO_BLOB_TEST_GENERIC_0006 - FAILED (4 ms)
1 of 6 FAILED
5 of 6 PASSED
Test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX) - FAILED (926 ms)

#### Summary ####
Test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX) - FAILED (926 ms)

#### EL2GO blob tests finished ####
```

# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# EdgeLock 2GO Import Blob

This sample application shows how to import encrypted EdgeLock 2GO secure object blobs from flash to the Secure storage(ITS). The imported objects can then be validated by executing crypto operations.

Blob Size:
- **[K4W1]** '2173 or 0x87D' bytes.

## Prerequisites

- Any serial communicator
- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- EdgeLock 2GO secure object blobs stored in the devices flash memory
- ** [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK) or standalone Provisioning Firmware

*NOTE: The Python scripts referred to in this example can be found in the [scripts](../../../../middleware/nxp_iot_agent/tst/el2go_blob_test/scripts) folder in the example directory.*

## Hardware requirements

- K32W148-EVK or KW45B41Z-EVK or KW45B41Z-LOC or FRDM-MCXW71 board
- USB-C cable
- Personal Computer

## Board settings

- **[K4W1]** The board must be provisioned with an OEM Auth Key Hash.

## Preparing the application

1.  Create device specific blobs:

    - Inline mode:
        1. Obtain a RTP JSON file from EdgeLock 2GO containing the desired blobs for your board
        2. **[K4W1]** Connect your board and run the file trough the preprocessor:
            ```sh
            el2go_blob_test_pre.py [RTP_JSON_PATH]
            ```
    - **[MCXN]** Memory mode:
        1. Create a device group in EdgeLock 2GO containing the desired blobs for your board
        2. Provision the blobs to your board via the `el2go-host` app from SPSDK (set the `secure_objects_address` property of the config file to `0x000C0000`):
            ```sh
            el2go-host provision-device -p [COM_PORT] --config [CONFIG_PATH] --workspace [WORKSPACE_PATH]
            ```
        3. Run the RTP JSON file downloaded by `el2go-host` trough the preprocessor, specifying the memory location method and address:
            ```sh
            el2go_blob_test_pre.py [WORKSPACE_PATH]/provisionings.json --storage_mode memory --blob_address 0x000C0000
            ```
        *NOTE: This method only works for a maximum of 16 blobs, not exceeding 16KB in total.*

    *ATTENTION: Make sure the lifecycle and OEM Auth Key Hash of your blobs match the one provisioned to the board. Attempting to rewrap/provision blobs with an OEM CLOSED lifecycle to an OEM OPEN board will change the lifecycle!*

2.  **[OPTIONAL]** Enable all possible variations:

    By default, the test suite only runs variations that are expected to pass for a given board. If you want to run all possible tests instead, you can specify that:

    - **[META]** By disabling the Kconfig symbol `RUN_VERIFIED_ONLY`
    - **[IDE]** By defining `RUN_VERIFIED_ONLY` as `0` in `mcux_config.h`

    *Note: If you input an entirely unsupported blob, the testcase will still run and fail, even if `RUN_VERIFIED_ONLY` is enabled.*

3.  **[OPTIONAL]** Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionally, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[K4W1]** Application note [AN14109 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW45) ("Documentation->Secure Files" section).

4.  Build the application:

    - **[META]** Compile the *el2go_blob_test* project with your desired toolchain.
    - **[IDE]** Compile the *el2go_blob_test* project with your desired toolchain.

5.  Connect the USB-C (K32W148-EVK or KW45B41Z-EVK or KW45B41Z-LOC or FRDM-MCXW71) cable to the PC host and the MCU-Link USB port (J14 [KW45B41Z-EVK or KW45B41Z-LOC]) on the board.

6.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7.  Download the application to the target board:

    - **[META]** `west flash` will download the image. 
    - **[IDE]** Downloading the image.

8.  Press the reset button on the board or launch the debugger in your IDE to run the application.

9. **[OPTIONAL]** Convert results to JUnit:

    If you capure the console output of the application, you can feed it into the postprocessor to recieve the results in the JUnit format:

    ```sh
    el2go_blob_test_post.py [CONSOLE_OUTPUT_PATH] [JUNIT_OUT_PATH]
    ```


## Running the application

The log below shows the output of the application in the terminal window.

#### Start EL2GO blob tests ####
Running test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001000
  Description: 'External MASTER_KEY AES256 DECRYPT ALL_AEAD'
  Test EL2GO_BLOB_TEST_GENERIC_03001000 - PASSED (106 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001001
  Description: 'External MASTER_KEY AES256 CRYPT ALL_AEAD'
  Test EL2GO_BLOB_TEST_GENERIC_03001001 - PASSED (28 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001002
  Description: 'External MASTER_KEY AES256 ENCRYPT ALL_AEAD'
  Test EL2GO_BLOB_TEST_GENERIC_03001002 - PASSED (30 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001006
  Description: 'External MASTER_KEY AES256 DECRYPT ALL_CIPHER'
  Test EL2GO_BLOB_TEST_GENERIC_03001006 - PASSED (31 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001007
  Description: 'External MASTER_KEY AES256 CRYPT ALL_CIPHER'
  Test EL2GO_BLOB_TEST_GENERIC_03001007 - PASSED (34 ms)
> Executing test EL2GO_BLOB_TEST_GENERIC_03001008
  Description: 'External MASTER_KEY AES256 ENCRYPT ALL_CIPHER'
  Test EL2GO_BLOB_TEST_GENERIC_03001008 - PASSED (35 ms)
6 of 6 PASSED
Test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX) - PASSED (264 ms)

#### Summary ####
Test suite GENERIC (EL2GO_BLOB_TEST_GENERIC_XXXX) - PASSED (264 ms)

#### EL2GO blob tests finished ####
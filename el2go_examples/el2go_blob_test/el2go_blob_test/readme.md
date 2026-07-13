# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# EdgeLock 2GO Blob Test

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
- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- EdgeLock 2GO secure object blobs stored in the devices flash memory
- ** [SPSDK](https://www.nxp.com/design/design-center/software/development-software/secure-provisioning-sdk-spsdk:SPSDK) or standalone Provisioning Firmware

*NOTE: The Python scripts referred to in this example can be found in the [scripts](../../../../middleware/nxp_iot_agent/tst/el2go_blob_test/scripts) folder in the example directory.*

## Hardware requirements

- KW45B41Z-EVK or FRDM-MCXW71 or MCXW71-EVK or KW47-EVK or FRDM-MCXW72 or MCXW72-EVK board
- USB-C cable
- Personal Computer

## Board settings

- **[KW45]**, **[MCX W71]**, **[KW47]**, **[MCX W72]** The board must be provisioned with an OEM Auth Key Hash.

## Preparing the application

1.  Create device specific blobs:

    - Inline mode:
        1. Obtain a RTP JSON file from EdgeLock 2GO containing the desired blobs for your board
        2. **[KW45]**, **[MCX W71]**, **[KW47]**, **[MCX W72]** Connect your board and run the file trough the preprocessor:
            ```sh
            el2go_blob_test_pre.py [RTP_JSON_PATH]
            ```
    *ATTENTION: Make sure the lifecycle and OEM Auth Key Hash of your blobs match the one provisioned to the board. Attempting to rewrap/provision blobs with an OEM CLOSED lifecycle to an OEM OPEN board will change the lifecycle!*

2.  **[OPTIONAL]** Enable all possible variations:

    By default, the test suite only runs variations that are expected to pass for a given board. If you want to run all possible tests instead, you can specify that:

    - **[GITHUB]** By disabling the Kconfig symbol `RUN_VERIFIED_ONLY`
    - **[ZIP]** By defining `RUN_VERIFIED_ONLY` as `0` in `mcux_config.h`

    *Note: If you input an entirely unsupported blob, the testcase will still run and fail, even if `RUN_VERIFIED_ONLY` is enabled.*

3.  **[OPTIONAL]** Enable secure boot:

    This feature is optional for **[KW45]**, **[MCX W71]**, **[KW47]**, **[MCX W72]**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionally, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[KW45]** Application note [AN14109 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW45) ("Documentation->Public Files" section).
    - **[MCX W71]**, Application note [AN14371 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/MCX-W71X) ("Documentation->Public Files" section).
    - **[KW47]**, Application note [AN14371 "Secure Boot Using the SEC Tool"](https://www.nxp.com/products/KW47) ("Documentation->Public Files" section).
    - **[MCX W72]**, Application note [AN14613 Secure Boot using the SEC Tool"](https://www.nxp.com/products/MCX-W72X) ("Documentation->Public Files" section).

4.  Build the application:

    - **[GITHUB]** Compile the *el2go_blob_test* project with your desired toolchain.
    - **[ZIP]** Compile the *el2go_blob_test* project with your desired toolchain.

5.  Connect the PC host to the MCU-Link USB port on the board.

6.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7.  Download the application to the target board:

    - **[GITHUB]** `west flash` will download the image. 
    - **[ZIP]** Downloading the image.

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
## Supported Boards
- MCX-W71-EVK
- KW47-EVK
- FRDM-MCXW72
- FRDM-MCXW71
- KW45B41Z-EVK
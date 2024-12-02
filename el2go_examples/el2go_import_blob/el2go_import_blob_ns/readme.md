# EdgeLock 2GO Import Blob (NS)

This demo demonstrates how to import the encrypted EdgeLock 2GO secure object blobs from Flash to the
ITS storage. The imported objects are used for executing crypto operations.

The workspace structure (when building this order must be respected):
- el2go_import_blob_s: project running in Secure processing environment (SPE)
- el2go_import_blob_ns: project running in Non-secure processing environment (NSPE)

The application requires to have encrypted EdgeLock 2GO secure object blobs loaded in flash. This can be achieved
using offline provisioning via the SPSDK (el2go-host tool, provision-device command).
The flash location where the EdgeLock 2GO blobs should be stored by el2go-host is 0x084B0000 [RW61X] or 0x001C0000 [MCXN].
You can specify this in the template file of el2go-host via the "additional_data_address" property.

## Prerequisites

- Active EdgeLock 2GO account (https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO)
- Any Serial communicator

## Hardware requirements

- FRDM-RW612, RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N95XX-EVK board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N95XX-EVK) cable
- Personal Computer

## Board settings

No special settings are required.

## Prepare the Demo

1.  By default the validation of blobs is disabled. It can be enabled by adjusting the following macro:

    in file middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/el2go_import_blob.h
    #define VALIDATE_PSA_IMPORT_OPERATION 1
    
    This provides an example on how the imported blobs can be used. Specifically, the example demonstrates:
    - AES-ECB message encryption with a 256-bit key.
    - ECDSA SHA 256 message signing.

2.  To correctly run the example on RW61X, the secure boot mode on the device needs to be enabled. For MCXN, this is not required.
    The bootheader needs to be removed from the SPE image, it has to be merged with the NSPE image and the resulting image must be 
    signed with the OEM key. Additionaly, if the example is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted
    with the OEM FW encryption key and loaded as an SB3.1 container.
    Details on how to execute these steps can be found in the following documents:
    - RW61x: Application note AN13813 "Secure boot on RW61x", downloadable from
      https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612
      in the "Secure Files" section.
    - MCXN: Application note AN14148 "Enabling Secure boot and Trust Provisioning on MCX N series", downloadable from
      https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-n-series-microcontrollers/mcx-n94x-54x-highly-integrated-multicore-mcus-with-on-chip-accelerators-intelligent-peripherals-and-advanced-security:MCX-N94X-N54X
      in the "Secure Files" section.

3.  [Optional] In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case
    you want to support bigger blobs (8K is the maximum size supported by PSA), you need to change the following two macros:

    in file middleware/tfm/tf-m/platform/ext/target/nxp/$board$/config_tfm_target.h
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00

    in file middleware/tfm/tf-m/platform/ext/target/nxp/$board$/partition/flash_layout.h
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)

4.  [Optional] The default flash location of the EdgeLock 2GO blobs is 0x084B0000 [RW61X] or 0x001C0000 [MCXN].
    In case you want to change this flash location, you need to adjust the following macros:

    in file middleware/tfm/tf-m/platform/ext/target/nxp/$board$/partition/flash_layout.h
    #define TFM_EL2GO_NV_DATA_IMPORT_ADDR (0x084B0000)
	
    in file middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/el2go_psa_import.h
    #define BLOB_AREA 0x084B0000
	
    Please make sure that your choice does not overlap with any other flash regions.

5.  Compile the projects using the dedicated toolchain in the following order:
    - el2go_import_blob_s
    - el2go_import_blob_ns

6.  Connect the USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA, MCX-N5XX-EVK or MCX-N95XX-EVK) cable to the PC host and the MCU-Link USB port
    (J10 [FRDM-RW612], J7 [RD-RW61X-BGA] or J5 [MCX-N5XX-EVK or MCX-N95XX-EVK]) on the board.

7.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8. Download the program to the target board.

9. Launch the debugger in your IDE to begin running the example.

## Running the demo

The log below shows the output of the demo in the terminal window:

Booting in SPE project, TF-M initialization:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.7.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jump to NSPE project, import of blobs from Flash to PSA ITS storage:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
2 blob(s) imported from flash successfully
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Validation of the blobs, encryption for AES Master key, sign operation for ECC key pair:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Validate imported blobs

 Cipher encrypt passed!

 ECC sign passed!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards

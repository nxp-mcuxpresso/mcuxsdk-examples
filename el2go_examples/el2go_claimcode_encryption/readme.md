# EdgeLock 2GO Claim Code Encryption

This sample application shows how to create and store an encrypted claim code blob on MCU devices.

It is meant to be used together with the [el2go_agent](../el2go_agent/el2go_agent_ns/readme.md) application, which needs to be downloaded afterwards. It will read the claim code blob and provide it to the EdgeLock 2GO service during device connection; the EdgeLock 2GO service will then decrypt and validate the blob and claim the device if the corresponding claim code is found in one of the device groups.

## Prerequisites

- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- Any serial communicator

## Setup of the EdgeLock 2GO service

The documentation which explains how to setup an EdgeLock 2GO account to

- Create a device group
- Create and copy a claim code for the device group
- Get the EdgeLock 2GO claim code public key (RW61x_Claiming_Public_Key)

can be found on the Edgelock 2GO web interface under the "Documentation" tab.

## Hardware requirements

- FRDM-RW612 or RD-RW61X-BGA board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable
- Personal Computer

## Board settings

No special settings are required.

## Preparing the application

1.  Provide the plain EdgeLock 2GO claim code string as created on the EdgeLock 2GO service:

    [middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h](../../../middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h)

    ```c
    #define IOT_AGENT_CLAIMCODE_STRING "insert_claimcode_from_el2go"
    ```

2.  Provide the EdgeLock 2GO claim code public key as obtained from the EdgeLock 2GO service (in the format of a hexadecimal byte array): 

    [middleware/nxp_iot_agent/inc/nxp_iot_agent_config_credentials.h](../../../middleware/nxp_iot_agent/inc/nxp_iot_agent_config_credentials.h)

    ```c
    #define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_PROD
    #define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_PROD_SIZE
    ```

3.  **[OPTIONAL]** Set the flash location of the claim code:

    By default, the encrypted claim code will be written to `0x084A0000`. This can be changed by adjusting a varibale.

    [middleware/nxp_iot_agent/ex/src/apps/el2go_claimcode_encryption.c](../../../middleware/nxp_iot_agent/ex/src/apps/el2go_claimcode_encryption.c)

    ```c
    #define CLAIM_CODE_INFO_ADDRESS
    ```

    *ATTENTION: Don't forget to align the location in the [el2go_agent](../el2go_agent/el2go_agent_ns/readme.md) application if you plan to use the two applications together.*

4.  **[OPTIONAL]** Enable secure boot:

    If the application should be used on a device with secure boot mode enabled, the bootheader needs to be removed from the image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).

5.  Build the application.

6.  Connect the USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612] or J7 [RD-RW61X-BGA]) on the board.

7.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

8.  Download the application to the target board.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window.

```
Enabling ELS... done
Generating random ECC keypair... done
Calculating shared secret... done
Creating claimcode blob... done
claimcode (*): *** dynamic data ***
claimcode (*): *** dynamic data ***
claimcode (*): *** dynamic data ***
claimcode information written to flash at address 0x 84a0000
```

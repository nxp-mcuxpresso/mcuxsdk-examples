# EdgeLock 2GO Agent (NS)

This sample application shows how to use the EdgeLock 2GO service to provisioning keys and certificates to an MCU device. Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

Workspace structure:
- *el2go_agent_s*: Project running in the secure processing environment (S)
- *el2go_agent_ns*: Project running in the non-secure processing environment (NS)

The application supports two modes for registering a device at the EdgeLock 2GO service:
- UUID registration: At startup, the device UUID will be printed, which can be used for manual registration.
- Claiming: The [el2go_claimcode_encryption](../../el2go_claimcode_encryption/readme.md) application must be run before, which will store a claim code blob in the flash memory. The EL2GO Agent application will present the claim code to the EdgeLock 2GO service and automatically register the device.

## Prerequisites

- Active [EdgeLock 2GO](https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO) account
- Any serial communicator

## Setup of the EdgeLock 2GO service

The documentation which explains how to setup the EdgeLock 2GO Account to

- Create a device group and whitelist the device UUID
- Create and copy a claim code for the device group
- Create secure objects
- Assign the secure objects to the device group

can be found on the Edgelock 2GO web interface under the "Documentation" tab.

## Hardware requirements

- FRDM-RW612 or RD-RW61X-BGA board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable
- Personal Computer

## Board settings

No special settings are required.

## Preparing the application

1.  Provide the EdgeLock 2GO URL for the account (can be found in the "Admin" section):

    [middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h](../../../../middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h)

    ```c
    #define EDGELOCK2GO_HOSTNAME
    ```

2.  Provide the Wi-Fi access point credentials:

    [middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c](../../../../middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c)

    ```c
    #define AP_SSID
    #define AP_PASSWORD
    ```

3.  **[OPTIONAL]** Enable claim code injection:

    In case you want to use the "Claiming" registration mode, enable the corresponding macro.

    [middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h](../../../../middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h)

    ```c
    #define IOT_AGENT_CLAIMCODE_INJECT_ENABLE 1
    ```

    *ATTENTION: If you changed the default location in the [el2go_claimcode_encryption](../../el2go_claimcode_encryption/readme.md) application, you need to align the location here.*

    [middleware/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_inject.c](../../../../middleware/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_inject.c)

    ```c
    #define CLAIM_CODE_INFO_ADDRESS
    ```

4.  **[OPTIONAL]** Enable MQTT demo:

    In case you want to use provisioned ECC key pairs and corresponding X.509 certificates to execute TLS mutual-authentication and MQTT message exchange with AWS and/or Azure clouds, enable the corresponding macro.
    
    [middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h](../../../../middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h)

    ```c
    #define IOT_AGENT_MQTT_ENABLE 1
    ```

    In the same file, the AWS and/or Azure macros should be set to the object IDs as defined at the EdgeLock 2GO service.

    ```c
    #define AWS_SERVICE_KEY_PAIR_ID
    #define AWS_SERVICE_DEVICE_CERT_ID
    ...
    #define AZURE_SERVICE_KEY_PAIR_ID
    #define AZURE_SERVICE_DEVICE_CERT_ID
    ```

    The settings of other macros are server dependent and their meaning can be found in the AWS/Azure documentation. By default, the demo is executing a connection to both clouds when `IOT_AGENT_MQTT_ENABLE` is enabled; To enable or disable them individually, use the `AWS_ENABLE` and `AZURE_ENABLE` macros respectively.

5.  **[OPTIONAL]** Enable support for large blobs:

    In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case you want to support larger blobs (8K is the maximum size supported by PSA), you need to adjust two TF-M parameters.

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/config_tfm_target.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/config_tfm_target.h)

    ```c
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00
    ```

    [middleware/tfm/tf-m/platform/ext/target/nxp/[BOARD]/partition/flash_layout.h](../../../../middleware/tfm/tf-m/platform/ext/target/nxp/frdmrw612/partition/flash_layout.h)

    ```c
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)
    ```

6.  Enable secure boot:

    To correctly run the application, the secure boot mode on the device needs to be enabled.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).

7.  Build the application:

    - **[META]** Compile the *el2go_agent_ns* project with your desired toolchain using `--sysbuild`.
    - **[IDE]** First compile the *el2go_agent_s* project and then the *el2go_agent_ns* project.

8.  Connect the USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612] or J7 [RD-RW61X-BGA]) on the board.

9.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

10. Download the application to the target board:

    - **[META]** `west flash` will download both the S and the NS image. 
    - **[IDE]** Downloading the S image will also download the NS image.

11. Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window (with MQTT connection to Azure service).

Booting the S project (TF-M initialization):

```
[INF] Beginning TF-M provisioning
[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.8.0
```

Jumping to the NS project, loading the Wi-Fi firmware and connecting to an access point:

```
Initializing Wi-Fi connection...

MAC Address: 00:50:43:02:FF:01
PKG_TYPE: BGA
Set BGA tx power table data
[i] Successfully initialized Wi-Fi module

Connecting as client to ssid: XXX

[i] Connected to Wi-Fi
ssid: XXX
```

Initializing the EdgeLock 2GO Agent and printing the UUID:

```
Performance timing: DEVICE_INIT_TIME : 11659ms
Start
UID in hex format: 351D5D247008FE41B5A6ACD45A55AD92
UID in decimal format: 70601549536478234583756656856983252370
```

Connecting to the EdgeLock 2GO service and printing the report:

```
Updating device configuration from [xxx.device-link.edgelock2go.com]:[443].
Update status report:
  The device update was successful (0x0001: SUCCESS)
  The correlation-id for this update is 2b971c9c-b046-4b7b-8062-ca5bc13f7b20.
  Status for remote trust provisioning: 0x0001: SUCCESS.
    On endpoint 0x70000010, for object 0x00004100, status: 0x0001: SUCCESS.
    On endpoint 0x70000010, for object 0x00004101, status: 0x0001: SUCCESS.
Found configuration data for 0 services.
Performance timing: ENTIRE_SESSION_TIME : 5306ms
        Performance timing: AGENT_INIT_TIME : 28ms
        Performance timing: TLS_PREP_TIME : 14ms
        Performance timing: NETWORK_CONNECT_TIME : 1230ms
        Performance timing: PROCESS_PROVISION_TIME : 3990ms
        CRL_TIME : [65ms] and COMMAND_TXRX_TIME : [0ms] included in PROCESS_PROVISION_TIME
```

Connecting to an Azure service using the provisioned ECC key pair and corresponding X.509 certificate:

```
MQTT attempting to register Azure Service 'azuretest-0000000000a82ac3-0000' (the operation migth take around 1 minute)...
[INFO] (Network connection b2990) TLS handshake successful.
[INFO] MQTT connection established with the broker.bal.azure-devices-provisioning.net established.
Subscription ack message received
[INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
[INFO] State record updated. New state=MQTTPublishDone.
Publish message received on topic
Device State is now ASSIGNING
[INFO] De-serialized incoming PUBLISH packet: DeserializerResult=MQTTSuccess.
[INFO] State record updated. New state=MQTTPublishDone.
Publish message received on topic
Device State is now ASSIGNED
State is ASSIGNED
[INFO] Disconnected from the broker.
[INFO] (Network connection b2990) TLS close-notify sent.
Attempt 0 for connecting to Azure service 'azuretest-0000000000a82ac3-0000'...
[INFO] (Network connection b2688) TLS handshake successful.
[INFO] (Network connection b2688) Connection to IotHub-DL-EL2GO-E2ETests.azure-devices.net establis[INFO] MQTT connection established with the broker.
Echo successfully published
Echo successfully published
Echo successfully published
Echo successfully published
[INFO] Disconnected from the broker.
[INFO] (Network connection b2688) TLS close-notify sent.
```

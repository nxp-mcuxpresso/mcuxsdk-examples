# EdgeLock 2GO Agent (NS)

This demo demonstrates how to use the EdgeLock 2GO service for provisioning keys and certificates into the MCU device.
Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

The workspace structure (when building this order must be respected):
- el2go_agent_s: project running in Secure processing environment (SPE)
- el2go_agent_ns: project running in Non-secure processing environment (NSPE)

The demo supports two modes for registering device to the EdgeLock 2GO service:
- UUID registration: at the start the demo is printing the UUID which can be used for registering it on the service
- Claiming: the EdgeLock 2GO Claim Code Encryption must be run in front, which will store the claim code blob
in the Flash memory. The EdgeLock 2GO Agent example will present the claim code to the EdgeLock 2GO service
and automatically register the device.

The device on which the example is running must have the secure boot enabled, otherwise the blob verification and
decryption keys can't be derived.

## Prerequisites

- Active EdgeLock 2GO account (https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO)
- Any Serial communicator

## Setup of the EdgeLock 2GO platform

The documentation which explains how to setup the EdgeLock 2GO Account to
- Create device group and whitelist device UUID
- Create and copy claim code for device group
- Create Secure Object
- Assign Secure Objects to device
can be found under the EdgeLock 2GO account under the Documentation tab.

## Hardware requirements

- FRDM-RW612 or RD-RW61X-BGA board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable
- Personal Computer

## Board settings

No special settings are required.

## Prepare the Demo

1.  Provide the EdgeLock 2GO URL for the account (is in Admin settings section)

    in file middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h
    #define EDGELOCK2GO_HOSTNAME

2.  Provide the wifi access point credentials

    in file middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c
    #define AP_SSID
    #define AP_PASSWORD

3.  [Optional] Only in case of claiming registration method the set to 1 the macro

    in file middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h
    #define IOT_AGENT_CLAIMCODE_INJECT_ENABLE 1

    The Flash address from where the claim code will be read from Flash is set
    at 0x084A0000; this can be change by manipulating the following variable. The value should
    match the one used in EdgeLock 2GO Claim Code Encryption

    in file middleware/nxp_iot_agent/ex/src/utils/iot_agent_claimcode_inject.c
    #define CLAIM_CODE_INFO_ADDRESS

4.  [Optional] Only in case the user wants to use provisioned ECC key pairs and corresponding X.509 certificates
    to execute TLS mutual-authentication and MQTT message exchange with AWS and/or Azure clouds, set to 1 the macro:

    in file middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h
    #define IOT_AGENT_MQTT_ENABLE 1

    In same file the following macros should be set to the object ID as defined on EdgeLock 2GO service:
    #define $SERVER$_SERVICE_KEY_PAIR_ID
    #define $SERVER$_SERVICE_DEVICE_CERT_ID

    Setting of other macros is server dependent and the meaning can be found on AWS/Azure documentation.
    By default the demo is executing the connection to both clouds when IOT_AGENT_MQTT_ENABLE is enabled;
    To enable or disable them individually, use the AWS_ENABLE and AZURE_ENABLE macros respectively.

5.  [Optional] In order to maximize the TF-M ITS performance, the maximum supported blob size is set to 2908 bytes. In case
    you want to support bigger blobs (8K is the maximum size supported by PSA), you need to change the following two macros:

    in file middleware/tfm/tf-m/platform/ext/target/nxp/$board$/config_tfm_target.h
    #define ITS_MAX_ASSET_SIZE 3 * 0xC00

    in file middleware/tfm/tf-m/platform/ext/target/nxp/$board$/partition/flash_layout.h
    #define TFM_HAL_ITS_SECTORS_PER_BLOCK (3)

6.  To correctly run the example, the secure boot mode on the device needs to be enabled. The bootheader needs to be removed
    from the SPE image, it has to be merged with the NSPE image and the resulting image must be signed with the OEM key.
    Additionaly, if the example is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with
    the OEM FW encryption key and loaded as an SB3.1 container.
    Details on how to execute these steps can be found in the Application note AN13813 "Secure boot on RW61x", downloadable from
    https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612
    in the "Secure Files" section.

7.  Compile the projects using the dedicated toolchain in the following order:
    - el2go_agent_s
    - el2go_agent_ns

6.  Connect the USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable to the PC host and the MCU-Link USB port
    (J10 [FRDM-RW612] or J7 [RD-RW61X-BGA]) on the board.

9.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

10. Download the Wifi FW as described in components/conn_fwloader/readme_rc.txt

11. Download the program to the target board.

12. Launch the debugger in your IDE to begin running the example.

## Running the demo

The log below shows the output of the demo in the terminal window (with MQTT connection to Azure service).

Booting in SPE project, TF-M initialization:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[INF] Beginning TF-M provisioning
[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.7.0
Creating an empty ITS flash layout.
Creating an empty PS flash layout.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jump to NSPE project, load of Wifi FW and connection to access point:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing Wi-Fi connection...

MAC Address: 00:50:43:02:FF:01
PKG_TYPE: BGA
Set BGA tx power table data
[i] Successfully initialized Wi-Fi module

Connecting as client to ssid: XXX

[i] Connected to Wi-Fi
ssid: Galaxy XXX
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Initialization of the EdgeLock 2GO Agent and printing of the UUID:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Performance timing: DEVICE_INIT_TIME : 11659ms
Start
UID in hex format: 351D5D247008FE41B5A6ACD45A55AD92
UID in decimal format: 70601549536478234583756656856983252370
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connection to EdgeLock 2GO service and printing of the report:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connection to Azure service using the provisioned ECC key pair and corresponding X.509 certificate:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards

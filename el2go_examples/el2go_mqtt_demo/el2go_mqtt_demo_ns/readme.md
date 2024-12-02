# EdgeLock 2GO MQTT Demo (NS)

This demo demonstrates how to use MQTT connections to cloud services such as AWS or Azure with preprovisioned keys
and certificates from the EdgeLock 2GO service.

The workspace structure (when building this order must be respected):
- el2go_mqtt_demo_s: project running in Secure processing environment (SPE)
- el2go_mqtt_demo_ns: project running in Non-secure processing environment (NSPE)

The device on which the example is running must have the secure boot enabled, otherwise the blob verification and
decryption keys can't be derived.

## Prerequisites

- Any Serial communicator
- EdgeLock 2GO keypairs and certificates for AWS and/or Azure already imported to the device (ITS).
  This can be achieved via offline provisioning (with el2go_import_blob application) or via online
  provisioning (with el2go_agent application). Please refer to their repspective readmes on
  how to perform the provisioning and import.

ATTENTION: It is important not to erase the ITS part of the flash (0x83C0000 to 0x83E0000) when flashing this application,
as this is where the EdgeLock 2GO objects are stored.

## Hardware requirements

- FRDM-RW612 or RD-RW61X-BGA board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable
- Personal Computer

## Board settings

No special settings are required.

## Prepare the Demo

1.  Provide the EdgeLock 2GO object IDs of the keys and certificates already imported to the device as well as the
    AWS and/or Azure connection parameters in the file middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h
    (the relevant macros are enclosed by "doc: MQTT required modification - start" and "doc: MQTT required modification - end").
    Details on the different configuration options are explained in the file.

2.  Provide the wifi access point credentials

    in file middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c
    #define AP_SSID
    #define AP_PASSWORD

3.  To correctly run the example, the secure boot mode on the device needs to be enabled. The bootheader needs to be removed
    from the SPE image, it has to be merged with the NSPE image and the resulting image must be signed with the OEM key.
    Additionaly, if the example is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with
    the OEM FW encryption key and loaded as an SB3.1 container.
    Details on how to execute these steps can be found in the Application note AN13813 "Secure boot on RW61x", downloadable from
    https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612
    in the "Secure Files" section.

4.  Compile the projects using the dedicated toolchain in the following order:
    - el2go_mqtt_demo_s
    - el2go_mqtt_demo_ns

5.  Connect the USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable to the PC host and the MCU-Link USB port
    (J10 [FRDM-RW612] or J7 [RD-RW61X-BGA]) on the board.

6.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7. Download the Wifi FW as described in components/conn_fwloader/readme_rc.txt

8. Download the program to the target board.

9. Launch the debugger in your IDE to begin running the example.

## Running the demo

The log below shows the output of the demo in the terminal window (Connecting to both AWS and Azure).

Booting in SPE project, TF-M initialization:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.7.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jump to NSPE project, load of Wifi FW and connection to access point:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Initializing Wi-Fi connection... 

Wi-Fi cau temperature : 28
MAC Address: 00:50:43:02:FF:01 
PKG_TYPE: BGA
Set BGA tx power table data 
[i] Successfully initialized Wi-Fi module

Connecting as client to ssid: XXX

[i] Connected to Wi-Fi
ssid: XXX
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connection to AWS service using the provisioned ECC key pair and corresponding X.509 certificate:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Attempt 1 for connecting to AWS service 'awstest-0000000000c04616-0000'...
[INFO] (Network connection 20058f00) TLS handshake successful.
[INFO] (Network connection 20058f00) Connection to aw9969rp3sm22-ats.iot.eu-central-1.amazonaws.com[INFO] MQTT connection established with the broker.
Echo successfully published
Echo successfully published
Echo successfully published
Echo successfully published
[INFO] Disconnected from the broker.
[INFO] (Network connection 20058f00) TLS close-notify sent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connection to Azure service using the provisioned ECC key pair and corresponding X.509 certificate:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MQTT attempting to register Azure Service 'azuretest-0000000000c04617-0000' (the operation migth take around 1 minute)...
[INFO] (Network connection 200589f0) TLS handshake successful.
[INFO] (Network connection 200589f0) Connection to global.azure-devices-provisioning.net establishe[INFO] MQTT connection established with the broker.
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
[INFO] (Network connection 200589f0) TLS close-notify sent.
Attempt 0 for connecting to Azure service 'azuretest-0000000000c04617-0000'...
[INFO] (Network connection 200586e8) TLS handshake successful.
[INFO] (Network connection 200586e8) Connection to IotHub-DL-EL2GO-E2ETests.azure-devices.net estab[INFO] MQTT connection established with the broker.
Echo successfully published
Echo successfully published
Echo successfully published
Echo successfully published
[INFO] Disconnected from the broker.
[INFO] (Network connection 200586e8) TLS close-notify sent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

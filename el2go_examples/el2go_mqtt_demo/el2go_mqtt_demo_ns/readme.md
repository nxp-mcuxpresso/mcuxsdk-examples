# EdgeLock 2GO MQTT Demo (NS)

This sample application shows how to use MQTT connections to cloud services such as AWS or Azure with preprovisioned keys and certificates from the EdgeLock 2GO service.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *el2go_mqtt_demo_s*: Project running in the secure processing environment (S)
- *el2go_mqtt_demo_ns*: Project running in the non-secure processing environment (NS)

Additional information about EdgeLock 2GO Provisioning for MCUs can be found in AN14624 under the following link https://www.nxp.com/webapp/Download?colCode=AN14624&amp;location=null.

## Definitions

- **[GITHUB]**: refers to the case where the MCU SDK is cloned as GitHub Repository and the EdgeLock 2GO application is build using meta build sytem.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/repo.html.
- **[ZIP]**: refers to the case whete MCU SDK is generated as ZIP package and the EdgeLock 2GO application is imported and build in one of supported IDEs.
  More details can be found in https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/package.html.

## Prerequisites

- Any serial communicator
- EdgeLock 2GO keypairs and certificates for AWS and/or Azure already imported to the device (TF-M ITS)
  - Via offline provisioning with the [el2go_import_blob](../../el2go_import_blob/el2go_import_blob_ns/readme.md) application
  - **[RW61X]** Via online provisioning with the [el2go_agent](../../el2go_agent/el2go_agent_ns/readme.md) application

*ATTENTION: It is important not to erase the ITS part of the flash (`0x083C0000` to `0x083E0000` [RW61X] or `0x00120000` to `0x00140000` [MCXN]) when flashing this application, as this is where the EdgeLock 2GO objects are stored.*

## Hardware requirements

- FRDM-RW612, RD-RW612-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK or FRDM-MCXN947 board
- FRDM-RW612, RD-RW612-BGA, MCX-N5XX-EVK, MCX-N9XX-EVK or FRDM-MCXN947T board
- USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
- USB-C (FRDM-RW612, FRDM-MCXN947T) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable
- Ethernet cable (FRDM-MCXN947, MCX-N5XX-EVK or MCX-N9XX-EVK)
- Ethernet cable (FRDM-MCXN947T, MCX-N5XX-EVK or MCX-N9XX-EVK)
- Personal Computer

## Board settings

### MCX-N5XX-EVK & MCX-N9XX-EVK

Jumper settings:
- JP13 2-3: Ethernet reference clock

### Other boards

No special settings are required.

## Preparing the application

1.  Provide the EdgeLock 2GO object IDs of the keys and certificates already imported to the device as well as the AWS and/or Azure connection parameters:

    [middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h](../../../../middleware/nxp_iot_agent/ex/inc/iot_agent_demo_config.h)

    ```c
    // doc: MQTT required modification - start
    ...
    // doc: MQTT required modification - end
    ```

    Details on the different configuration options are explained in the file.

    When creating the secure objects on EdgeLock 2GO, Custom policies should be chosen.
    Additionally for the ECC key pair following options should be selected:
    - Device Lifecycle should match the lifecycle of the device on which the application will run
    - Permitted algorithm should be set to ECDSA SHA 256
    - SIGN HASH usage should be selected
    For the X.509 certificate following options should be selected:
    - Device Lifecycle should match the lifecycle of the device on which the application will run
    - EXPORT usage should be selected

2.  **[RW61X]** Provide the Wi-Fi access point credentials:

    [middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c](../../../../middleware/nxp_iot_agent/ex/src/network/iot_agent_network_lwip_wifi.c)

    ```c
    #define AP_SSID
    #define AP_PASSWORD
    ```

3.  Enable secure boot:

    To correctly run the application on RW61X, the secure boot mode on the device needs to be enabled. **For MCXN, this is optional**.

    The bootheader needs to be removed from the S image, it has to be merged with the NS image and the resulting image must be signed with the OEM key. Additionaly, if the application is supposed to run in the OEM CLOSED life cycle, the image needs to be encrypted with the OEM FW encryption key and loaded as an SB3.1 container.

    Details on how to execute these steps can be found in the following documents:
    - **[RW61X]** Application note [AN13813 "Secure boot on RW61x"](https://www.nxp.com/products/wireless-connectivity/wi-fi-plus-bluetooth-plus-802-15-4/wireless-mcu-with-integrated-tri-radio-1x1-wi-fi-6-plus-bluetooth-low-energy-5-3-802-15-4:RW612) ("Documentation->Secure Files" section).
    - **[MCXN]** Application note [AN14148 "Enabling Secure boot and Trust Provisioning on MCX N series"](https://www.nxp.com/products/processors-and-microcontrollers/arm-microcontrollers/general-purpose-mcus/mcx-arm-cortex-m/mcx-n-series-microcontrollers/mcx-n94x-54x-highly-integrated-multicore-mcus-with-on-chip-accelerators-intelligent-peripherals-and-advanced-security:MCX-N94X-N54X) ("Documentation->Secure Files" section).

4.  Build the application:

    - **[GITHUB]** Compile the *el2go_mqtt_demo_ns* project with your desired toolchain using `--sysbuild`.
    - **[ZIP]** First compile the *el2go_mqtt_demo_s* project and then the *el2go_mqtt_demo_ns* project.

5.  Connect the USB-C (FRDM-RW612, FRDM-MCXN947) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612], J7 [RD-RW612-BGA], J5 [MCX-N5XX-EVK or MCX-N9XX-EVK] or J17 [FRDM-MCXN947]) on the board.
5.  Connect the USB-C (FRDM-RW612, FRDM-MCXN947T) or Micro-USB (RD-RW612-BGA, MCX-N5XX-EVK or MCX-N9XX-EVK) cable to the PC host and the MCU-Link USB port (J10 [FRDM-RW612], J7 [RD-RW612-BGA], J5 [MCX-N5XX-EVK or MCX-N9XX-EVK] or J17 [FRDM-MCXN947T]) on the board.

6.  Open a serial terminal with the following settings:

    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

7.  Prepare the network interface:

    - **[RW61X]** Download the Wi-Fi firmware as described in [components/conn_fwloader/readme.txt](../../../../components/conn_fwloader/readme.txt).
    - **[MCXN]** Connect the Ethernet cable to the RJ45 port.

8.  Download the application to the target board:

    - **[GITHUB]** `west flash` will download both the S and the NS image.
    - **[ZIP]** Downloading the S image will also download the NS image.

9.  Press the reset button on the board or launch the debugger in your IDE to run the application.

## Running the application

The log below shows the output of the application in the terminal window (connecting to both AWS and Azure).

Booting the S project (TF-M initialization):

```
Booting TF-M v2.1.1
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
[INF][PS] Encryption alg: 0x5500100
```

**[RW61X]** Jumping to the NS project, loading the Wi-Fi firmware and connecting to an access point:

```
Initializing Wi-Fi connection...

Wi-Fi cau temperature : 28
MAC Address: 00:50:43:02:FF:01
PKG_TYPE: BGA
Set BGA tx power table data
[i] Successfully initialized Wi-Fi module

Connecting as client to ssid: XXX

[i] Connected to Wi-Fi
ssid: XXX
```

**[MCXN]** Jumping to the NS project and connecting to the network via Ethernet:

```
Initializing PHY ...
```

Connecting to an AWS service using the provisioned ECC key pair and corresponding X.509 certificate:

```
Attempt 1 for connecting to AWS service 'awstest-0000000000c04616-0000'...
[INFO] (Network connection 20058f00) TLS handshake successful.
[INFO] (Network connection 20058f00) Connection to aw9969rp3sm22-ats.iot.eu-central-1.amazonaws.com[INFO] MQTT connection established with the broker.
Echo successfully published
Echo successfully published
Echo successfully published
Echo successfully published
[INFO] Disconnected from the broker.
[INFO] (Network connection 20058f00) TLS close-notify sent.
```

Connecting to an Azure service using the provisioned ECC key pair and corresponding X.509 certificate:

```
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
```

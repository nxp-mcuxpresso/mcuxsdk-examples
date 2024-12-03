# azure_http

## Overview
Azure_http demo shows basic communication with iot-hub via http protocol.

## Running the demo
After the board is flashed the Tera Term will start printing the state of event bits.

Example output:
Initializing PHY...
 DHCP state       : SELECTING
 DHCP state       : REQUESTING
 DHCP state       : CHECKING
 DHCP state       : BOUND

 IPv4 Address     : xxx.xxx.xxx.xxx
 IPv4 Subnet mask : xxx.xxx.xxx.xxx
 IPv4 Gateway     : xxx.xxx.xxx.xxx

Info: Initializing SNTP
Info: SNTP initialization complete
Starting the IoTHub client sample HTTP...
IoTHubClient_LL_SetMessageCallback...successful.
IoTHubClient_LL_SendEventAsync accepted message [0] for transmission to IoT Hub.
Confirmation[0] received for message tracking id = 0 with result = IOTHUB_CLIENT_CONFIRMATION_OK
IoTHubClient_LL_SendEventAsync accepted message [1] for transmission to IoT Hub.
Confirmation[1] received for message tracking id = 1 with result = IOTHUB_CLIENT_CONFIRMATION_OK
IoTHubClient_LL_SendEventAsync accepted message [2] for transmission to IoT Hub.
Confirmation[2] received for message tracking id = 2 with result = IOTHUB_CLIENT_CONFIRMATION_OK
IoTHubClient_LL_SendEventAsync accepted message [3] for transmission to IoT Hub.
Confirmation[3] received for message tracking id = 3 with result = IOTHUB_CLIENT_CONFIRMATION_OK
IoTHubClient_LL_SendEventAsync accepted message [4] for transmission to IoT Hub.
Confirmation[4] received for message tracking id = 4 with result = IOTHUB_CLIENT_CONFIRMATION_OK


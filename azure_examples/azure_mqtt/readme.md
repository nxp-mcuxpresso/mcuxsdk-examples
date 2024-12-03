# azure_mqtt

## Overview
Azure_mqtt demo shows basic communication with iot-hub via mqtt protocol.

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
IoTHubClient_LL_SetMessageCallback...successful.
IoTHubClient_LL_SendEventAsync accepted message [0] for transmission to IoT Hub.
-> 15:56:54 CONNECT | VER: 4 | KEEPALIVE: 240 | FLAGS: 192 | USERNAME: MCU-IOT-Hub.azure-devices.net/frdmk64f/api-version=2016-11-14&DeviceClientType=iothubclient%2f1.2.3%20(native%3b%20FreeRTOS%3b%20MSDK) | PWD: XXXX | CLEAN: 0
IoTHubClient_LL_SendEventAsync accepted message [1] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [2] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [3] for transmission to IoT Hub.
IoTHubClient_LL_SendEventAsync accepted message [4] for transmission to IoT Hub.
<- 15:56:54 CONNACK | SESSION_PRESENT: true | RETURN_CODE: 0x0
-> 15:56:54 SUBSCRIBE | PACKET_ID: 2 | TOPIC_NAME: devices/frdmk64f/messages/devicebound/# | QOS: 1
-> 15:56:54 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=true&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 3 | PAYLOAD_LEN: 74
-> 15:56:54 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=true&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 4 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 5 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 6 | PAYLOAD_LEN: 74
-> 15:56:55 PUBLISH | IS_DUP: false | RETAIN: 0 | QOS: DELIVER_AT_LEAST_ONCE | TOPIC_NAME: devices/frdmk64f/messages/events/temperatureAlert=false&%24.cid=CORE_ID&%24.mid=MSG_ID&%24.ct=application%2Fjson&%24.ce=utf-8 | PACKET_ID: 7 | PAYLOAD_LEN: 74
<- 15:56:55 SUBACK | PACKET_ID: 2 | RETURN_CODE: 1
<- 15:56:56 PUBACK | PACKET_ID: 3
Confirmation[0] received for message tracking id = 0 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 4
Confirmation[1] received for message tracking id = 1 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 5
Confirmation[2] received for message tracking id = 2 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 6
Confirmation[3] received for message tracking id = 3 with result = IOTHUB_CLIENT_CONFIRMATION_OK
<- 15:56:56 PUBACK | PACKET_ID: 7
Confirmation[4] received for message tracking id = 4 with result = IOTHUB_CLIENT_CONFIRMATION_OK





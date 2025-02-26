# w_uart_host

## Overview
This section describes the implemented profiles and services, user interactions, and testing methods for the Wireless UART Host application.
The Wireless UART Host application is a demonstration of the Extended NBU architectural concept, where the Bluetooth LE Host Stack and the Link Layer both run on the NBU core, while the user-facing application runs on the Application core. The communication between the application and the Host is done via the FSCI protocol running on the inter-core RPMSG transport.

The Wireless UART Host application must be flashed together with the NCP FSCI Blackbox application.

## Implemented profiles and services
The Wireless UART application provides a shell interface through which the user can configure a black box to setup and exercise functionalities identical to the regular Wireless UART application. Both the GATT client and server for the custom Wireless UART profile and services are supported:
- Wireless UART Service (UUID: 01ff0100-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.0


The Wireless UART service is a custom service that implements a writable ASCII Char characteristic (UUID: 01ff0101-ba5e-f4ee-5ca1-eb1e5e4b1ce0) that holds the character written by the peer device.
Upon startup, the application is in the idle state. Section 5.14.4 describes the shell commands used to interact with the application.

## Supported Boards
- MCX-W72-EVK
- FRDM-MCXW-72
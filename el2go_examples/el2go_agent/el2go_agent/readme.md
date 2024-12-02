# EdgeLock 2GO Agent

This demo demonstrates how to use the EdgeLock 2GO service for provisioning keys and certificates into the SE05x secure element.
Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

## Prerequisites

- Active EdgeLock 2GO account (https://www.nxp.com/products/security-and-authentication/secure-service-2go-platform/edgelock-2go:EDGELOCK-2GO)
- Any Serial communicator

## Setup of the EdgeLock 2GO platform

The documentation which explains how to setup the EdgeLock 2GO Account to
- Add devices to the group
- Create Secure Object
- Assign Secure Objects to device
can be found under the EdgeLock 2GO account under the Documentation tab.

## Hardware requirements

- MIMXRT1060-EVKB, MIMXRT1060-EVKC or MIMXRT1170-EVKB board
- Micro-USB cable
- Personal Computer
- Network cable RJ45 standard (Network with Internet access)

## Board settings (MIMXRT1060-EVKB & MIMXRT1060-EVKC)

Jumper settings for OM-SE050ARD & OM-SE051ARD:
- J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
- J15 3-4 (Default): I2C target SDA connection
- J17 3-4 (Default): I2C target SCL connection

## Board settings (MIMXRT1170-EVKB)

Jumper settings for OM-SE050ARD & OM-SE051ARD:
- J14 3-4 (Default): EdgeLock SE05x_VCC pin routed to SE05X_VOUT pin
- J15 1-2: I2C target SDA connection
- J17 1-2: I2C target SCL connection

NOTE: Network cable needs to be connected to the 1G ENET port

## SE050 settings

- Connect OM-SE050ARD to the board.
- The SE050 on board requires keys & certificates provisioned for cloud connectivity demos.
- Please consult SE050 user manual and Plug & Trust middleware documentation, which is present in the
  Plug & Trust MW package, for further details. It can be downloaded from
  https://www.nxp.com/products/security-and-authentication/authentication/:SE050?tab=Design_Tools_Tab

## Building the Demo

Before you start building the demo you must configure the EdgeLock 2GO URL for your account:
- From the EdgeLock 2GO account copy the account specific URL (in Admin Settings section)
- Open file middleware/nxp_iot_agent/inc/nxp_iot_agent_config.h
- Fill the definition EDGELOCK2GO_HOSTNAME with the account specific URL.

In order to enable MQTT test to cloud services or configuring the device with specific device features,
refer to the documentation present in the Plug & Trust MW package and which can be downloaded from
https://www.nxp.com/products/security-and-authentication/authentication/:SE050?tab=Design_Tools_Tab

## Running the Demo

If you have built a binary, flash the binary on to the board and reset the board.

## Supported Boards

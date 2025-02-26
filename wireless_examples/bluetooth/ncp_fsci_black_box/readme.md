# ncp_fsci_black_box

## Overview
The Bluetooth LE FSCI Black Box demo application gives access to the Bluetooth LE Host Stack via a serial
interface using the FSCI protocol. See the FSCI (Framework Serial Communication Interface) manual for the
format of the FSCI commands and a full list of supported commands.

The demo can be used with the Test Tool for Connectivity Products. Command Console application can
be downloaded from the NXP website or using a custom application that supports the FSCI protocol and
commands.

This application is made to run on Core 1 of the supported platforms and can be excersised in conjunction with fsci_bridge or w_uat_host running on Core 0.

## Supported Boards
- MCX-W72-EVK
- FRDM-MCXW-72
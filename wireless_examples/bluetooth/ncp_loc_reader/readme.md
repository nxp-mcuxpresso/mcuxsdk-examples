# ncp_loc_reader

## Overview
The BLE ncp loc reader application is a simple demonstration program for core 1 based on the MCUXpresso SDK.
This application implements a BLE device that performs distance measurement using Channel Sounding.

The application is made to be used together with the corresponding loc_reader_host application on core 0.

The device handles all BLE related operations (connection, channel sounding, service discovery) and sends
the local and remote Channel Sounding data to the Core 0 application for processing.

## Supported Boards
- MCX-W72-EVK
- FRDM-MCXW-72
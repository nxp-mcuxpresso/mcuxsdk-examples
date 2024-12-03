# eatt_peripheral

## Overview
The EATT Peripheral application implements a GATT server and the following profiles and services:
- Battery Service v1.0
- Device Information Service v1.1
- Generic Attribute Profile

The Generic Attribute Profile includes the Server Supported Features characteristics, which is used to indicate
EATT support to the peer, and the Client Supported Features characteristic, which is used by the peer to
indicate its own EATT support.
The application behaves as a GAP peripheral node. It enters GAP General Discoverable Mode and waits for a
GAP central node to connect. The application implements two custom services, Service A and Service B. After
the EATT connection is completed, the peer must enable indications for the two services to periodically receive
profile data over EATT.

## Supported Boards
- KW47-EVK
- FRDM-MCXW71
- K32W148-EVK
- MCX-W72-EVK
- KW45B41Z-EVK
# digital_key_device

## Overview
The Digital Key Device with Channel Sounding application emulates a Car Connectivity Consortium (CCC) Digital Key Release 3-enabled smartphone.
It acts as a Bluetooth Low Energy central device, scanning for advertising coming from a Digital Key Car Anchor.

For the purposes of localization, the Digital Key Device acts as either a Channel Sounding Initiator or Reflector and as a Ranging Responder/Server in the context of the Ranging Service (RAS). The RAS server stores the local distance measurement data and provides it to the RAS client.

## Supported Boards
- KW47-EVK
- KW47-LOC
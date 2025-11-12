# digital_key_car_anchor_cs

## Overview
The Digital Key Car Anchor with Channel Sounding application implements one of the multiple Bluetooth Low Energy anchors that can reside inside a vehicle as part of the Car Connectivity Consortium (CCC) Digital Key Release 3 secure car access specification.
The anchor is a Bluetooth Low Energy peripheral which can perform both Legacy (1M PHY) and Extended Long
Range (500kbps Coded PHY) advertising depending on the scenario.
There can be multiple anchors inside a car. The anchors must act as a single device and share information between them
such as addresses, bonding data, or application-specific keys. The information must be shared such that a
CCC Digital Key enabled device such as a smartphone can connect to any one of the anchors, depending on positioning and not detect any difference.

For the purposes of localization, the Digital Key Car Anchor acts either as a Channel Sounding Initiator or Reflector and as a Ranging Requester in the context of the Ranging Service. The application runs the distance measurement algorithm using the local and remote distance measurement data.

## Supported Boards
- KW47-EVK
- KW47-LOC
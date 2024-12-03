# digital_key_car_anchor

## Overview
The Digital Key Car Anchor application implements one of the multiple Bluetooth Low Energy anchors that can reside inside a vehicle
as part of the Car Connectivity Consortium (CCC) Digital Key Release 3 secure car access specification.
The anchor performs a dual role. Inside the CCC Digital Key scope, it is a
Bluetooth Low Energy peripheral. This peripheral can perform both Legacy (1M PHY) and Extended Long
Range (500kbps Coded PHY) advertising depending on the scenario. It can also perform searching
for a CCC Digital Key-enabled device such as a smartphone.
Simultaneously, it can also act as a Bluetooth Low Energy Central, scanning for non-CCC key fobs. There can
be multiple anchors inside a car. The anchors must act as a single device and share information between them
such as addresses, bonding data, or application-specific keys. The information must be shared such that a
smartphone can connect to any one of the anchors, depending on positioning and not detect any difference.

## Supported Boards
- KW45B41Z-EVK
- KW47-EVK
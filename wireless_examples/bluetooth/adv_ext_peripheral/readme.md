# Low-power Extended Advertising Peripheral

## Demo functionality overview
The application behaves as a GAP peripheral node. It enters GAP General Discoverable Mode and waits for a
GAP central node to connect and configure notifications for the temperature value.

## Implemented profile and services
The adv_ext_peripheral application implements a GATT server, a custom profile and the following services.
- Temperature Service (UUID: 01ff0200-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.1

The Temperature service is a custom service that implements the Temperature characteristic (UUID: 0x2A6E)
with a Characteristic Presentation Format descriptor (UUID: 0x2904), both defined by the Bluetooth SIG.

## Supported platforms
The following platforms support the Extended Advertising Peripheral application:
- KW45B41Z-EVK
- K32W148-EVK
- FRDM-MCXW71
- KW47-EVK
- MCX-W72-EVK
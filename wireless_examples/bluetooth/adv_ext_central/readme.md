# Low-power Extended Advertising Central

## Demo functionality overview
The application behaves as a GAP central node. It enters GAP Limited Discovery Procedure and searches
for peripherals devices to pair with. After pairing with the peripheral, it configures notifications and displays
temperature values on a terminal connected to the UART port.

## Implemented profile and services
The adv_ext_central application implements a GATT client or server for the following profile and services.
- Temperature Service (UUID: 01ff0200-ba5e-f4ee-5ca1-eb1e5e4b1ce0)
- Battery Service v1.0
- Device Information Service v1.1

The Temperature service is a custom service that implements the Temperature characteristic (UUID: 0x2A6E)
with a Characteristic Presentation Format descriptor (UUID: 0x2904), both defined by the Bluetooth SIG.

## Supported platforms
The following platforms support the Extended Advertising Central application:
- KW45B41Z-EVK
- K32W148-EVK
- FRDM-MCXW71
- KW47-EVK
- MCX-W72-EVK
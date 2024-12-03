# Bluetooth LE Shell

## Demo functionality overview
The Bluetooth LE Shell Application implements a console application that allows the user to interact with a full feature Bluetooth Low Energy stack library. It implements all GAP roles and both GATT client and server.
Enabling these roles can be done using shell commands.

## Implemented profile and services
The application implements a dynamic GATT database. The user can add services at runtime and also erase the database contents. The database is always populated with the GAP and GATT services. These services cannot be erased. The user can dynamically add the following services:
- Heart Rate Service (UUID: 0x180D)
- Battery Service (UUID: 0x180F)
- Device Information Service (UUID: 0x180A)
- Internet Support Profile Service (0x1820)

## Supported platforms
The following platforms support the Bluetooth LE Shell application:
- KW45B41Z-EVK
- K32W148-EVK
- FRDM-MCXW71
- KW47-EVK
- FRDM-MCXW72
- MCX-W72-EVK
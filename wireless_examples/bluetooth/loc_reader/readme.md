# Localization Reader

## Demo functionality overview
The Localization Reader application is a Bluetooth Low Energy peripheral that implements the Ranging Service
(RAS) client side. This application runs the localization algorithm using the local and remote measurement
data received through RAS. Both the initiator and reflector Channel Sounding roles are supported and can be
configured at compile time or run time. In the default configuration, the reflector role is used.

For more information, please consult AN13974 "Bluetooth Low Energy Localization Application Note".

## Supported platforms
The following platforms support the Localization Reader application:
- KW47-EVK
- FRDM-MCXW72
- MCX-W72-EVK
- KW47-LOC
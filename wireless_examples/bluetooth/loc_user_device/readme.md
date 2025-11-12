# Localization User Device

## Demo functionality overview
The Localization User Device acts as a Bluetooth Low Energy central that implements the RAS server side.
Both the initiator and reflector Channel Sounding roles are supported and can be configured at compile or run time.
In the default configuration, the initiator role is used. The RAS server stores local measurement data and sends it
to the RAS client. The Localization User Device does not run the localization algorithms.

For more information, please consult AN13974 "Bluetooth Low Energy Localization Application Note".

## Supported platforms
The following platforms support the Localization User Device application:
- KW47-EVK
- FRDM-MCXW72
- MCX-W72-EVK
- KW47-LOC

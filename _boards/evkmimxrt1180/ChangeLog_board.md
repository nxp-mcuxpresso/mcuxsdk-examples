# Board Support Files

## board

### [25.06.00]
- Initial version
    - Replaced the XXX_SERIES macros with the CPU_xxx macros in the common configuration file
    - Added the legacy IAR supporting Mac file
### [26.09.00]
- Added BOARD_EarlyInit() which move MPU/cache configuration on CM7 core at very beginning of startup.
- Empty BOARD_ConfigMPU() on CM7 core since the configuration already moved forward into BOARD_EarlyInit().

## clock\_config

### [25.06.00]
- Initial version
    - Updated the clock_config.c to align with the latest part number and the clock tool model
    - Removed the OSC RC 400M dating control, which is not supported by the hardware

## pin\_mux

### [25.06.00]
- Initial version
    - Updated pin_mux.c to align with the latest part number and the pin tool model


# Board Support Files

## board

### [26.09.00]
- Added BOARD_EarlyInit() which move MPU/cache configuration on CM7 core at very beginning of startup.
- Empty BOARD_ConfigMPU() on CM7 core since the configuration already moved forward into BOARD_EarlyInit().

### [25.06.00]
- Initial version
    - Added IW610_2LL_USD module support
    - Updated display_support project segment

## clock\_config

### [25.06.00]
- Initial version
    - Updated clock_config.c to align with latest the part number and the clock tool model
    - Changed the default DCDC mode to CCM mode

## pin\_mux

### [25.06.00]
- Initial version
    - Updated pin_mux.c to align with the latest part number
    - Fixed the pad configuration slew rate polarity description

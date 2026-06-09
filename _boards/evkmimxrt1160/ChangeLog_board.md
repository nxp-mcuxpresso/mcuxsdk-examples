# Board Support Files

## board

### [25.06.00]
- Initial version
### [26.09.00]
- Added BOARD_EarlyInit() which move MPU/cache configuration on CM7 core at very beginning of startup.
- Empty BOARD_ConfigMPU() on CM7 core since the configuration already moved forward into BOARD_EarlyInit().

## clock\_config

### [25.06.00]
- Initial version

## pin\_mux

### [25.06.00]
- Initial version

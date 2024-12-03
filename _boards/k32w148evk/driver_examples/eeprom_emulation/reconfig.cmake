
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/driver_examples/eeprom_emulation/./hardware_init.c
            examples/_boards/kw45b41zevk/driver_examples/eeprom_emulation/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/driver_examples/eeprom_emulation/.
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_add_macro(
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1\
       -DEE_DATA_VALUE_REMAINING_PART=16\
       -DFLASH_PGM_SIZE=16"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

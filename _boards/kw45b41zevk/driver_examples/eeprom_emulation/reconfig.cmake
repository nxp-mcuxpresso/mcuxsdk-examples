
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

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/iar/KW45B41Z83xxxA_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/gcc/KW45B41Z83xxxA_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/iar/KW45B41Z83xxxA_ram.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/gcc/KW45B41Z83xxxA_ram.ld
)

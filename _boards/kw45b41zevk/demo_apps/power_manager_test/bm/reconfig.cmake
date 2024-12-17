
mcux_add_macro(
    CC "-DGENERIC_LIST_LIGHT=1\
       -DFSL_PM_SUPPORT_ALWAYS_ON_SECTION=1"
)

mcux_add_armgcc_configuration(
    CC "-DAPP_BYPASS_ECC_INIT=1"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW45B41Z83/iar/KW45B41Z83xxxA_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW45B41Z83/gcc/KW45B41Z83xxxA_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW45B41Z83/iar/KW45B41Z83xxxA_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW45B41Z83/gcc/KW45B41Z83xxxA_flash_rom_boot.ld
)
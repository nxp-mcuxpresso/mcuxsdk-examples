mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW43B43ZC7/iar/KW43B43ZC7_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW43B43ZC7/gcc/KW43B43ZC7_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW43B43ZC7/iar/KW43B43ZC7_flash_ce.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW43B43ZC7/gcc/KW43B43ZC7_flash_ce.ld
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW47B42ZB7/iar/kw47b42zb_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW47B42ZB7/gcc/kw47b42zb_cm33_core0_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW47B42ZB7/iar/kw47b42zb_cm33_core0_flash_ce.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/KW/KW47B42ZB7/gcc/kw47b42zb_cm33_core0_flash_ce.ld
)


mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXW/MCXW727C/iar/mcxw727_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXW/MCXW727C/gcc/mcxw727_cm33_core0_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXW/MCXW727C/iar/mcxw727_cm33_core0_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXW/MCXW727C/gcc/mcxw727_cm33_core0_flash_rom_boot.ld
)

mcux_add_macro(
    CC "-DDEMO_PD_MODE_NOT_OK=1"
    AS "-DECC_RAM_STCM8_INIT"
)

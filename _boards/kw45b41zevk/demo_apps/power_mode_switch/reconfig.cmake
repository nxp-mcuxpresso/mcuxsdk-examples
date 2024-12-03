
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_mode_switch/peripherals.c
            examples/_boards/${board}/demo_apps/power_mode_switch/peripherals.h
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

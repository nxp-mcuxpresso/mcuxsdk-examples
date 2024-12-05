
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.c
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.h
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
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/iar/KW45B41Z83xxxA_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/KW/KW45B41Z83/gcc/KW45B41Z83xxxA_flash_rom_boot.ld
)

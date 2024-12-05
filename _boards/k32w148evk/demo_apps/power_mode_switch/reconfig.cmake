
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.c
            ${board_root}/${board}/demo_apps/power_mode_switch/peripherals.h
            ${board_root}/kw45b41zevk/demo_apps/power_mode_switch/hardware_init.c
            ${board_root}/kw45b41zevk/demo_apps/power_mode_switch/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/demo_apps/power_mode_switch
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/K32W/K32W1480/iar/K32W1480xxxA_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/K32W/K32W1480/gcc/K32W1480xxxA_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/K32W/K32W1480/iar/K32W1480xxxA_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/Wireless/K32W/K32W1480/gcc/K32W1480xxxA_flash_rom_boot.ld
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/demo_apps/power_manager_test/bm/hardware_init.c
            examples/_boards/kw45b41zevk/demo_apps/power_manager_test/bm/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/demo_apps/power_manager_test/bm
)

mcux_add_macro(
    CC "-DGENERIC_LIST_LIGHT=1\
       -DFSL_PM_SUPPORT_ALWAYS_ON_SECTION=1"
)


mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/K32W/K32W1480/iar/K32W1480xxxA_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/K32W/K32W1480/gcc/K32W1480xxxA_flash.ld
)


mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/K32W/K32W1480/iar/K32W1480xxxA_flash_rom_boot.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/Wireless/K32W/K32W1480/gcc/K32W1480xxxA_flash_rom_boot.ld
)
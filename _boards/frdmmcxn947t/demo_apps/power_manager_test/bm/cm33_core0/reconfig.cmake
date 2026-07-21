
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/power_manager_test/bm
)

mcux_add_macro(
    CC "-DFSL_PM_SUPPORT_ALWAYS_ON_SECTION=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN947T/iar/MCXN947T_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN947T/gcc/MCXN947T_cm33_core0_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN947T/arm/MCXN947T_cm33_core0_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_manager_test/linker/MCXN947T_cm33_core0_flash_for_power_manager.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_manager_test/linker/MCXN947T_cm33_core0_flash_for_power_manager.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_manager_test/linker/MCXN947T_cm33_core0_flash_for_power_manager.scf
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager_test/startup/startup_gcc.S
    TOOLCHAINS armgcc
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager_test/startup/startup_iar.s
    TOOLCHAINS iar
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager_test/startup/startup_arm.S
    TOOLCHAINS mdk
)


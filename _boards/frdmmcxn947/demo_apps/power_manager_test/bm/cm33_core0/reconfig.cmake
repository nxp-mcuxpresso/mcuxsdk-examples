
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/power_manager_test/bm
)

mcux_add_macro(
    CC "-DFSL_PM_SUPPORT_ALWAYS_ON_SECTION=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXN/MCXN947/iar/MCXN947_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXN/MCXN947/gcc/MCXN947_cm33_core0_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXN/MCXN947/arm/MCXN947_cm33_core0_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXN947_cm33_core0_flash_for_power_manager.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXN947_cm33_core0_flash_for_power_manager.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXN947_cm33_core0_flash_for_power_manager.scf
)

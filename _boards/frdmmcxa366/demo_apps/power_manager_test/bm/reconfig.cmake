
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/${board}/demo_apps/power_manager_test/bm
)

mcux_add_macro(
    CC "-DFSL_PM_SUPPORT_ALWAYS_ON_SECTION=1"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA366/iar/MCXA366_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA366/gcc/MCXA366_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA366/arm/MCXA366_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA366_power_manager_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA366_power_manager_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA366_power_manager_flash.scf
)


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
    LINKER devices/MCX/MCXA/MCXA276/iar/MCXA276_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA276/gcc/MCXA276_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/MCX/MCXA/MCXA276/arm/MCXA276_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA276_power_manager_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA276_power_manager_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_manager_test/linker/MCXA276_power_manager_flash.scf
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/demo_apps/power_manager_test/bm/startup_MCXA276_power_manager.S
  TOOLCHAINS armgcc
)

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${device_root}/${soc_portfolio}/${soc_series}/${device}
        SOURCES iar/startup_${CONFIG_MCUX_HW_DEVICE_CORE}.s
        TOOLCHAINS iar
        CONFIG true
)

mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${device_root}/${soc_portfolio}/${soc_series}/${device}
        SOURCES arm/startup_${CONFIG_MCUX_HW_DEVICE_CORE}.S
        TOOLCHAINS mdk
        CONFIG true
)

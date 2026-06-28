
# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_mode_switch/bm_dcdc/linker/MIMXRT1152xxxxx_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_mode_switch/bm_dcdc/linker/MIMXRT1152xxxxx_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/demo_apps/power_mode_switch/bm_dcdc/linker/MIMXRT1152xxxxx_ram.ld
)


mcux_remove_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--config_def=__use_flash64MB__=1"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1170/MIMXRT1176/iar/MIMXRT1176xxxxx_cm7_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1170/MIMXRT1176/arm/MIMXRT1176xxxxx_cm7_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT1170/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_ram.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/bm_pmic/linker/MIMXRT1176xxxxx_cm7_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/bm_pmic/linker/MIMXRT1176xxxxx_cm7_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/demo_apps/power_mode_switch/bm_pmic/linker/MIMXRT1176xxxxx_cm7_ram.ld
)

mcux_remove_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--config_def=__use_flash64MB__=1"
)

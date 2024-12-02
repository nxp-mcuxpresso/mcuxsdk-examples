mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-Og"
)

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DUSE_SDRAM\
       -DLWIP_ENET_FLEXIBLE_CONFIGURATION"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)
mcux_remove_armgcc_configuration(
    TARGETS debug flexspi_nor_debug
    CC "-O0"
)
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1160/MIMXRT1166/iar/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1160/MIMXRT1166/gcc/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)
mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1160/MIMXRT1166/arm/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_server/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_server/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_server/linker/MIMXRT1166xxxxx_cm7_flexspi_nor.ld
)

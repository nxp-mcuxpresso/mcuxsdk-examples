


# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-Og"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O0"
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flexspi_nor_debug flexspi_nor_release
        LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flexspi_nor_debug flexspi_nor_release
        LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/MIMXRT1152xxxxx_flexspi_nor.ld
)

# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_iar_configuration(
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flexspi_nor_debug flexspi_nor_release
        LINKER ${device_root}/RT/RT1040/MIMXRT1042/iar/MIMXRT1042xxxxx_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS flexspi_nor_debug flexspi_nor_release
        LINKER ${device_root}/RT/RT1040/MIMXRT1042/gcc/MIMXRT1042xxxxx_flexspi_nor.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1042xxxxx_flexspi_nor.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/MIMXRT1042xxxxx_flexspi_nor.ld
)

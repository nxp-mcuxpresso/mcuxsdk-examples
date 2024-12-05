

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
)


# Add additional configuration
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)


mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"aws_mbedtls_config.h\\\""
)


# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/iar/MIMXRT1064xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/arm/MIMXRT1064xxxxx_flexspi_nor.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1064/MIMXRT1064/gcc/MIMXRT1064xxxxx_flexspi_nor.ld
)


# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1064xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1064xxxxx_flexspi_nor.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1064xxxxx_flexspi_nor.ld
)

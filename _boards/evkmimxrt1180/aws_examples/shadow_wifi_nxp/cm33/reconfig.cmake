
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/aws_examples/common/mbedtls/mbedtls_user_config.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/aws_examples/common/mbedtls
             ${board_root}/${board}
)


# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ele_s400_mbedtls_config.h\\\"\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)



# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O0"
)
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)


# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1189/gcc/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1189/iar/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1180/MIMXRT1189/arm/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)


# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)

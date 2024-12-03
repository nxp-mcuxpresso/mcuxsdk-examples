
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/aws_examples/common/mbedtls/mbedtls_user_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/aws_examples/common/mbedtls
)

# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ele_s400_mbedtls_config.h\\\"\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DFSL_ETH_ENABLE_CACHE_CONTROL\
       -DETH_RX_TASK_PRIO=6"
)
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
    CC "--dlib_config full"
)
mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug
    CC "-O1"
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
    LINKER devices/RT/RT1180/MIMXRT1189/gcc/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/iar/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1180/MIMXRT1189/arm/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)


# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/aws_examples/common/linker/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)

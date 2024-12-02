
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/ota_examples/ota_mcuboot_client/mbedtls_user_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/ota_examples/ota_mcuboot_client
)


mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_remove_macro(
    CC "MBEDTLS_CONFIG_FILE"

)
mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ele_s400_mbedtls_config.h\\\"\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DFSL_ETH_ENABLE_CACHE_CONTROL"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-g"
    CX "-g"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1180/MIMXRT1189/iar/MIMXRT1189xxxxx_cm33_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1180/MIMXRT1189/gcc/MIMXRT1189xxxxx_cm33_flexspi_nor.ld
)

mcux_remove_mdk_linker_script(
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/RT/RT1180/MIMXRT1189/arm/MIMXRT1189xxxxx_cm33_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/linker/MIMXRT1189xxxxx_cm33_flexspi_nor_mcuboot.ld
)

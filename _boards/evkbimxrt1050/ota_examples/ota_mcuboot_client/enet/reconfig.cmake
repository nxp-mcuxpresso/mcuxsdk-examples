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

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DconfigTOTAL_HEAP_SIZE=81920"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
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
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/iar/MIMXRT1052xxxxx_flexspi_nor.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/gcc/MIMXRT1052xxxxx_flexspi_nor.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER devices/RT/RT1050/MIMXRT1052/arm/MIMXRT1052xxxxx_flexspi_nor.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1052xxxxx_flexspi_nor_mcuboot.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1052xxxxx_flexspi_nor_mcuboot.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1052xxxxx_flexspi_nor_mcuboot.ld
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support/mcuboot_app_support.c
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support/mcuboot_app_support.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.h
            examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning/flash_partitioning.c
            examples/_boards/${board}/ota_examples/mcuboot_opensource/sblconfig.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config/mcuboot_config.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config/mcuboot_logging.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/flash_map.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/sysflash/sysflash.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ota_examples/ota_mcuboot_client
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/app_support
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/mcuboot_config
             middleware/mcuboot_opensource/boot/nxp_mcux_sdk/include/sysflash
             examples/_boards/${board}/ota_examples/mcuboot_opensource
             examples/_boards/${board}/ota_examples/mcuboot_opensource/flash_partitioning
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
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
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1176xxxxx_cm7_flexspi_nor_mcuboot.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1176xxxxx_cm7_flexspi_nor_mcuboot.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT1176xxxxx_cm7_flexspi_nor_mcuboot.ld
)

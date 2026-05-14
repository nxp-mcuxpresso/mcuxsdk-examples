mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/ota_examples/ota_mcuboot_client/mbedtls_user_config.h
            middleware/mcuboot_opensource/boot/nxp_mcux_sdk/flashapi/flash_api.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/ota_mcuboot_client
)

mcux_add_macro(
    CC "-DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DENCRYPTED_XIP_IPED"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/iar/RW612_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/Wireless/RW/RW612/gcc/RW612_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/RW610_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_linker/RW610_flash.ld
)

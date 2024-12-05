
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/ota_examples/ota_mcuboot_client/mbedtls_user_config.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/ota_mcuboot_client
)


mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_add_macro(
    CC "-DMBEDTLS_USER_CONFIG_FILE=\\\"mbedtls_user_config.h\\\"\
       -DHIGH_SPEED_SDIO_CLOCK"
)

mcux_remove_mdk_configuration(
    TARGETS flash_release
    CC "-Oz"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_flash.icf
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_flash.ld
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_flash.scf
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/ota_examples/ota_mcuboot_client/linker/MIMXRT595Sxxxx_cm33_flash.ld
)


#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/lwip_config/lwipopts.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/lwip_config/lwippools.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/lwip_config/lwiphooks.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/wifi_config/wifi_config.h
            ${board_root}/${board}/wifi_examples/common/./hardware_init.c
            ${board_root}/${board}/wifi_examples/common/./app.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/wifi_config
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/lwip_config
             ${board_root}/${board}/wifi_examples/wifi_wpa_supplicant/cm33/freertos_config
)



mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DXIP_IMAGE\
       -DXIP_EXTERNAL_FLASH\
       -DUSE_RTOS=1\
       -DCONFIG_WPA_SUPP_CRYPTO_MBEDTLS_PSA\
       -DMBEDTLS_USER_CONFIG_FILE=\\\"wpa_supp_mbedtls_config.h\\\"\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_mdk_configuration(
    TARGETS flash_release
    CC "-Os"
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

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.scf
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)

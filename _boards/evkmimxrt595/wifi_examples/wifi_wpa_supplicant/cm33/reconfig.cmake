
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwippools.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwiphooks.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/FreeRTOSConfig.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/lwipopts.h
            middleware/wifi_nxp/example/wifi_wpa_supplicant/wifi_config/wifi_config.h
            examples/_boards/${board}/wifi_examples/common/./hardware_init.c
            examples/_boards/${board}/wifi_examples/common/./app.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/wifi_bt_config.c
            examples/_boards/${board}/wifi_bt_config.h
            examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_wpa_supplicant/wifi_config
             examples/_boards/${board}/wifi_examples/common/app_config
)



mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DXIP_IMAGE\
       -DXIP_EXTERNAL_FLASH\
       -DUSE_RTOS=1\
       -DMBEDTLS_CONFIG_FILE=\\\"wpa_supp_mbedtls_config.h\\\"\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT595Sxxxx_cm33_flash.scf
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


#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/wifi_cli/wifi_config/wifi_config.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            ${board_root}/${board}/wifi_examples/common/host_sleep.c
            ${board_root}/${board}/wifi_examples/common/host_sleep.h
            ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwippools.h
            ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwiphooks.h
            ${board_root}/${board}/wifi_examples/wifi_cli/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config/lwipopts.h
            ${board_root}/${board}/wifi_examples/wifi_cli/${core_id}/hardware_init.c
            ${board_root}/${board}/wifi_examples/wifi_cli/${core_id}/app.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_examples/common/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            ${board_root}/${board}/wifi_examples/common/app_config/app_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/wifi_cli
             ${board_root}/${board}/wifi_examples/common
             ${board_root}/${board}/wifi_examples/wifi_cli/wifi_config
             ${board_root}/${board}/wifi_examples/wifi_cli/${core_id}
             ${board_root}/${board}/wifi_examples/common/app_config
             ${board_root}/${board}/wifi_examples/wifi_cli/lwip_config
             ${board_root}/${board}/wifi_examples/wifi_cli/freertos_config
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)
mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

# Enable SLIM features
mcux_add_macro(
    CC "-DCONFIG_FREERTOS_LOW_MEMORY_FOOTPRINT=1\
       -DCONFIG_LWIP_LOW_MEM_FOOTPRINT=1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.scf
)

# Remove all default linker files
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)

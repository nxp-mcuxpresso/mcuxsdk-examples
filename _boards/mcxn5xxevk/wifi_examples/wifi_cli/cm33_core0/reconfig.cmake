
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwippools.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwiphooks.h
            middleware/wifi_nxp/example/wifi_cli/freertos_config/FreeRTOSConfig.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwipopts.h
            middleware/wifi_nxp/example/wifi_cli/wifi_config/wifi_config.h
            ${board_root}/${board}/wifi_examples/common/${core_id}/hardware_init.c
            ${board_root}/${board}/wifi_examples/common/${core_id}/app.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_cli/wifi_config
             ${board_root}/${board}/wifi_examples/common/${core_id}
             middleware/wifi_nxp/example/wifi_cli/lwip_config
             middleware/wifi_nxp/example/wifi_cli/freertos_config
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
    LINKER  ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER  ${board_root}/${board}/wifi_examples/common/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_wifi.scf
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

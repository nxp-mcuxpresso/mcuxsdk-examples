
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/common/pin_mux/cm33_core0/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwippools.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwiphooks.h
            middleware/wifi_nxp/example/wifi_cli/freertos_config/FreeRTOSConfig.h
            middleware/wifi_nxp/example/wifi_cli/lwip_config/lwipopts.h
            middleware/wifi_nxp/example/wifi_cli/wifi_config/wifi_config.h
            ${board_root}/${board}/wifi_examples/common/cm33_core0/hardware_init.c
            ${board_root}/${board}/wifi_examples/common/cm33_core0/app.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
             ${board_root}/${board}/wifi_examples/common/cm33_core0
             middleware/wifi_nxp/example/wifi_cli/wifi_config
             middleware/wifi_nxp/example/wifi_cli/lwip_config
             middleware/wifi_nxp/example/wifi_cli/freertos_config
)

mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x400\
        --config_def=__heap_size__=0x400"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x400\
        -Xlinker --defsym=__heap_size__=0x400"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x400\"\
        --predefine=\"-D__heap_size__=0x400\""
)



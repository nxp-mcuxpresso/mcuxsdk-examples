
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/wifi_test_mode/lwip_config/lwipopts.h
            middleware/wifi_nxp/example/wifi_test_mode/lwip_config/lwippools.h
            middleware/wifi_nxp/example/wifi_test_mode/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/common/pin_mux.c
            ${board_root}/${board}/wifi_examples/common/pin_mux.h
            middleware/wifi_nxp/example/wifi_test_mode/wifi_config/wifi_config.h
            ${board_root}/${board}/wifi_examples/common/./hardware_init.c
            ${board_root}/${board}/wifi_examples/common/./app.h
            ${board_root}/${board}/sdmmc_config.c
            ${board_root}/${board}/sdmmc_config.h
            ${board_root}/${board}/wifi_bt_config.c
            ${board_root}/${board}/wifi_bt_config.h
            ${board_root}/${board}/wifi_examples/common/app_config/app_config.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/wifi_examples/common
             middleware/wifi_nxp/example/wifi_test_mode/wifi_config
             ${board_root}/${board}/wifi_examples/common/app_config
             middleware/wifi_nxp/example/wifi_test_mode/lwip_config
             middleware/wifi_nxp/example/wifi_test_mode/freertos_config
)



mcux_add_iar_configuration(
    CX "--no_clustering"
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1"
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


#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/wifi_examples/uart_wifi_bridge/lwip_config/lwipopts.h
            ${board_root}/${board}/wifi_examples/uart_wifi_bridge/lwip_config/lwippools.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/mfg_wlan_bt_fw.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduart8987_mfg_wlan_bt.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduartIW416_mfg_wlan_bt.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduart_nw61x_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/uart_nw61x_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduartspi_iw610_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/uartspi_iw610_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sd_iw610_mfg_se.h
            ${board_root}/${board}/wifi_examples/uart_wifi_bridge/freertos_config/FreeRTOSConfig.h
            ${board_root}/${board}/wifi_examples/uart_wifi_bridge/wifi_config/wifi_config.h
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
    INCLUDES middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/wifi_config
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/lwip_config
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/freertos_config
             ${board_root}/${board}/wifi_examples/common/cm33_core0
)

mcux_add_source(
    SOURCES pin_mux.c
            pin_mux.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Os"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_release flexspi_nor_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DLPFLEXCOMM_INIT_NOT_USED_IN_DRIVER=1"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
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

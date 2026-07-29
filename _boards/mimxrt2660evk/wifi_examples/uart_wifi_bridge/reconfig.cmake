
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/wifi_examples/uart_wifi_bridge/lwip_config/lwipopts.h
            examples/_boards/${board}/wifi_examples/uart_wifi_bridge/lwip_config/lwippools.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/mfg_wlan_bt_fw.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduart8987_mfg_wlan_bt.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduartIW416_mfg_wlan_bt.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduart_nw61x_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/uart_nw61x_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sduartspi_iw610_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/uartspi_iw610_mfg_se.h
            middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware/sd_iw610_mfg_se.h
            examples/_boards/${board}/wifi_examples/uart_wifi_bridge/freertos_config/FreeRTOSConfig.h
            examples/_boards/${board}/wifi_examples/uart_wifi_bridge/wifi_config/wifi_config.h
            examples/_boards/${board}/wifi_examples/common/./hardware_init.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.c
            examples/_boards/${board}/wifi_examples/common/pin_mux.h
            examples/_boards/${board}/wifi_examples/common/./app.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/wifi_bt_config.c
            examples/_boards/${board}/wifi_bt_config.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/wifi_examples/common
             middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware
             examples/_boards/${board}/wifi_examples/uart_wifi_bridge/wifi_config
             examples/_boards/${board}/wifi_examples/uart_wifi_bridge/lwip_config
             examples/_boards/${board}/wifi_examples/uart_wifi_bridge/freertos_config
)

# Use the WiFi-scoped pin_mux (defines BOARD_InitUSDHC1Pins); drop board-common to avoid duplicate symbols.
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/common/pin_mux/pin_mux.c
            ${board_root}/${board}/common/pin_mux/pin_mux.h
)
mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/common/pin_mux
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS xspi_nor_release xspi_nor_debug
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DBOARD_USE_PCAL6524=1\
       -DBOARD_USE_PCA9555=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)
# USDHC1 SDIO DMA needs TRDC domain-0 authorization; board.h default (0) omits USDHC.
mcux_add_macro(
    CC "-DBOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN=1"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_release xspi_nor_debug
    LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_release xspi_nor_debug
    LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor.icf
)
# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_release xspi_nor_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_release xspi_nor_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor.icf
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

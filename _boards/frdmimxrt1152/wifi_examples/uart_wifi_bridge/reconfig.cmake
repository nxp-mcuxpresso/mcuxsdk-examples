
#
# Copyright 2026 NXP
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
    INCLUDES middleware/wifi_nxp/example/uart_wifi_bridge/mfg_wifi_bt_firmware
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/wifi_config
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/lwip_config
             ${board_root}/${board}/wifi_examples/uart_wifi_bridge/freertos_config
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
       -DBOARD_USE_PCAL6524=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_remove_mdk_configuration(
    TARGETS flexspi_nor_release
    CC "-Oz"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/gcc/MIMXRT1152xxxxx_flexspi_nor.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/iar/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${device_root}/RT/RT1150/MIMXRT1152/arm/MIMXRT1152xxxxx_flexspi_nor.scf
)

# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/wifi_examples/uart_wifi_bridge/linker/MIMXRT1152xxxxx_flexspi_nor.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/wifi_examples/uart_wifi_bridge/linker/MIMXRT1152xxxxx_flexspi_nor.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flexspi_nor_debug flexspi_nor_release
    LINKER ${board_root}/${board}/wifi_examples/uart_wifi_bridge/linker/MIMXRT1152xxxxx_flexspi_nor.scf
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

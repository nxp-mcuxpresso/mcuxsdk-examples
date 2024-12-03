#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wifi_webconfig LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wifi_examples/wifi_webconfig)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
        examples/wifi_examples/wifi_webconfig/webconfig.c
        examples/wifi_examples/wifi_webconfig/webconfig.h
        examples/wifi_examples/wifi_webconfig/httpsrv_fs_data.c
        examples/wifi_examples/wifi_webconfig/cred_flash_storage.c
        examples/wifi_examples/wifi_webconfig/cred_flash_storage.h
        examples/wifi_examples/wifi_webconfig/http_server.c
        examples/wifi_examples/wifi_webconfig/http_server.h

        examples/wifi_examples/wifi_webconfig/nxp/lwipopts.h
        examples/wifi_examples/wifi_webconfig/nxp/lwippools.h
        examples/wifi_examples/wifi_webconfig/wifi_config/wifi_config.h
        examples/wifi_examples/wifi_webconfig/FreeRTOSConfig.h

        examples/_boards/${board}/FreeRTOSConfigBoard.h
        examples/_boards/${board}/wifi_examples/common/app_config/app_config.h
        examples/_boards/${board}/wifi_examples/common/pin_mux.c
        examples/_boards/${board}/wifi_examples/common/pin_mux.h

        # WebUI files
        # components/edgefast_wifi/example/wifi_webconfig/webui/favicon.ico
        # components/edgefast_wifi/example/wifi_webconfig/webui/index.html
        # components/edgefast_wifi/example/wifi_webconfig/webui/NXP_logo.png
        # components/edgefast_wifi/example/wifi_webconfig/webui/webconfig.css
        # components/edgefast_wifi/example/wifi_webconfig/webui/webconfig.js
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES 
        examples/wifi_examples/wifi_webconfig/nxp
        examples/wifi_examples/wifi_webconfig/wifi_config/
        examples/wifi_examples/wifi_webconfig
        examples/_boards/${board}/wifi_examples/common/app_config
        examples/_boards/${board}/wifi_examples/common
)

# Add additional configuration
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DLWIP_NETIF_API=1\
       -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1\
       -DHTTPSRV_CFG_DEFAULT_SES_CNT=8\
       -DSDK_DEBUGCONSOLE_UART"
)
mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)

mcux_remove_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=2048\
        --config_def=__heap_size__=90000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=2048\"\
        --predefine=\"-D__heap_size__=90000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=2048\
        -Xlinker --defsym=__heap_size__=90000"
)

mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/wifi_webconfig/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_webconfig.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/wifi_webconfig/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_webconfig.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flash_debug flash_release
    LINKER examples/_boards/${board}/wifi_examples/wifi_webconfig/linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_webconfig.scf
)

include(${SdkRootDirPath}/examples/_boards/${board}/wifi_examples/wifi_webconfig/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(uart_wifi_bridge LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wifi_examples/uart_wifi_bridge)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/uart_wifi_bridge/main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wifi_nxp/example/uart_wifi_bridge
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)
#mcux_add_macro(
#    CC "-DCONFIG_BT_IND_DNLD"
#)


include(${SdkRootDirPath}/examples/_boards/${board}/wifi_examples/uart_wifi_bridge/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)


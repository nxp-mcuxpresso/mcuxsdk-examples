#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wifi_cli_prov LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wifi_examples/wifi_cli_prov)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/wifi_nxp/example/wifi_cli_prov/main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/wifi_nxp/example/wifi_cli_prov
)

include(${SdkRootDirPath}/examples/_boards/${board}/wifi_examples/wifi_cli_prov/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
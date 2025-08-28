#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_tickless LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/freertos_examples/freertos_tickless)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES freertos_tickless.c
)

mcux_add_include(
    INCLUDES .
)

include(${SdkRootDirPath}/${board_root}/${board}/freertos_examples/freertos_tickless/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

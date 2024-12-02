#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_tickless_lpc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_examples/freertos_tickless_lpc)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/_boards/${board}/freertos_examples/freertos_tickless_lpc/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES freertos_tickless_lpc.c
)

mcux_add_include(
    INCLUDES .
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_lpi2c LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_driver_examples/freertos_lpi2c)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES freertos_lpi2c.c
)

mcux_add_include(
    INCLUDES .
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

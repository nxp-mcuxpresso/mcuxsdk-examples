#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sema4_dualcore_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/sema4/dualcore/)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES sema4_core1.c
            ../demo_common.h
)

mcux_add_include(
    INCLUDES .
             ..
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

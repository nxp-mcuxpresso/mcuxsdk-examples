# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(v2x_base_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/v2x_base_examples/v2x_base_test)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(SOURCES v2x_base_test.c)
mcux_add_include(INCLUDES .)

mcux_add_linker_symbol(SYMBOLS "__stack_size__=0x800\ __heap_size__=0x600")

include(${SdkRootDirPath}/${board_root}/${board}/v2x_base_examples/v2x_base_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

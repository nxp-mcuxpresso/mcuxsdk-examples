# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(netc_switch_standalone LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/netc_switch_standalone)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/netc_switch_standalone/netc_switch_standalone.c
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=2048\
             __heap_size__=25600"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/netc_switch_standalone/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

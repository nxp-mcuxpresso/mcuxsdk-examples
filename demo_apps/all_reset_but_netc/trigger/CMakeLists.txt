# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(all_reset_but_netc_trigger LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/all_reset_but_netc/trigger)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/all_reset_but_netc/trigger/trigger.c
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=2048\
             __heap_size__=25600"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/all_reset_but_netc/trigger/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

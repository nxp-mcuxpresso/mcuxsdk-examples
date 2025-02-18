# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_iperf_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/lwip_examples/lwip_iperf/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_iperf/bm/lwip_iperf_bm.c
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1\
       -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x1000\
             __heap_size__=0x1000"
)

include(${SdkRootDirPath}/${board_root}/${board}/lwip_examples/lwip_iperf/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
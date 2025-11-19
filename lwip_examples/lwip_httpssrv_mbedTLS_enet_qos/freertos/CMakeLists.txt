# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_httpssrv_mbedTLS_enet_qos_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/lwip_examples/lwip_httpssrv_mbedTLS_enet_qos/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/httpsrv_common/fs_data/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/httpsrv_common/fs_data/webpage/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_httpssrv_mbedTLS/freertos/lwip_httpssrv_mbedTLS_freertos.c
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000\
             __heap_size__=110000"
)

include(${SdkRootDirPath}/${board_root}/${board}/lwip_examples/lwip_httpssrv_mbedTLS_enet_qos/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
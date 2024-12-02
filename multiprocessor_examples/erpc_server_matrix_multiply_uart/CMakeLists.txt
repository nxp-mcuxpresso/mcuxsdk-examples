#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(erpc_server_matrix_multiply_uart LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/multiprocessor_examples/erpc_server_matrix_multiply_uart)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/_boards/${board}/multiprocessor_examples/erpc_server_matrix_multiply_uart/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main_server.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __heap_size__=0x2000 \
            "
)

mcux_add_iar_configuration(
        CX "--no_exceptions --no_rtti"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

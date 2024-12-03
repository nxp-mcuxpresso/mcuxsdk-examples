#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(erpc_remote_control LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/multiprocessor_examples/erpc_remote_control)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/_boards/${board}/multiprocessor_examples/erpc_remote_control/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES remote_control.c
            service/erpc_remote_control_app_0_common.h
            service/erpc_remote_control_app_0_common.hpp
            service/erpc_remote_control_app_1_common.h
            service/erpc_remote_control_app_1_common.hpp
            service/erpc_remote_control_app_0_interface.hpp
            service/erpc_remote_control_app_0_interface.cpp
            service/erpc_remote_control_app_1_interface.hpp
            service/erpc_remote_control_app_1_interface.cpp
            service/erpc_remote_control_app_0_server.hpp
            service/erpc_remote_control_app_0_server.cpp
            service/c_erpc_remote_control_app_0_server.h
            service/c_erpc_remote_control_app_0_server.cpp
            service/erpc_remote_control_app_1_client.hpp
            service/erpc_remote_control_app_1_client.cpp
            service/c_erpc_remote_control_app_1_client.h
            service/c_erpc_remote_control_app_1_client.cpp
)

mcux_add_include(
    INCLUDES .
             service/
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

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_tzm_primary_core_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/ns)

include(${SdkRootDirPath}/CMakeLists.txt)

# This will point to: ${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/reconfig.cmake
# where common board multicore properties are set.
include(${CMAKE_CURRENT_LIST_DIR}/../../config.cmake)

# Include reconfigure for Non Secure Project
include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/ns/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main.c
            ../veneer_table.h
)

mcux_add_include(
    INCLUDES .
             ../
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/trustzone_examples/tzm_api/
)

mcux_add_configuration(
        CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __heap_size__=0x2000 \
             __use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../rpmsg_lite_pingpong_tzm_primary_core_s/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_s_CMSE_lib.o"
    TOOLCHAINS armgcc mdk iar
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_rtos_master_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/master)

include(${SdkRootDirPath}/CMakeLists.txt)

# This will point to: ${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/reconfig.cmake
# where common board multicore properties are set.
include(${CMAKE_CURRENT_LIST_DIR}/../../config.cmake)

include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/master/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/master/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES ../../rpmsg_lite_pingpong_rtos/primary/main.c
)

mcux_add_include(
    INCLUDES ../../rpmsg_lite_pingpong_rtos/primary
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

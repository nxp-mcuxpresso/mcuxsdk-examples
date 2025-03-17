#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_rtos_no_mcmgr_remote LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr)

include(${SdkRootDirPath}/CMakeLists.txt)

# This will point to: ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/reconfig.cmake
# where common board multicore properties are set.
include(${CMAKE_CURRENT_LIST_DIR}/../../config.cmake)

include(${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES ../../rpmsg_lite_pingpong_rtos/secondary/main.c
)

mcux_add_include(
    INCLUDES ../../rpmsg_lite_pingpong_rtos/secondary
)


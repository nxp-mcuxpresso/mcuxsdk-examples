#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_dsp LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

include(${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/${core_id}/reconfig.cmake OPTIONAL)

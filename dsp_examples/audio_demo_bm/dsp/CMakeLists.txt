#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_audio_demo_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/audio_demo_bm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm
    SOURCES dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/dsp_examples/audio_demo_bm
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/audio_demo_bm/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/dsp_examples/${core_id}.cmake)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_xaf_playback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/xaf_playback)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/xaf_playback/dsp/*.c
            examples/dsp_examples/xaf_playback/dsp/*.h
            examples/_boards/${board}/dsp_examples/xaf_playback/*.h
            examples/_boards/${board}/dsp_examples/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/xaf_playback/dsp
             examples/_boards/${board}/dsp_examples/xaf_playback
             examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}
             examples/_boards/${board}/dsp_examples
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/dsp_examples/${core_id}.cmake)

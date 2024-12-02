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
    SOURCES examples/dsp_examples/xaf_playback/cm/*.c
            examples/dsp_examples/xaf_playback/cm/*.h
            examples/dsp_examples/xaf_playback/resources/*.h
            examples/dsp_examples/common/mcu/*.c
            examples/dsp_examples/common/mcu/*.h
            examples/_boards/${board}/dsp_examples/*.c
            examples/_boards/${board}/dsp_examples/*.h
            examples/_boards/${board}/dsp_examples/xaf_playback/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/common/mcu
             examples/dsp_examples/xaf_playback/cm
             examples/dsp_examples/xaf_playback/resources
             examples/_boards/${board}
             examples/_boards/${board}/dsp_examples
             examples/_boards/${board}/dsp_examples/xaf_playback
             examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}
)


include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

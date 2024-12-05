#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_xaf_usb_demo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/dsp_examples/xaf_usb_demo)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/xaf_usb_demo/dsp/*.c
            examples/dsp_examples/xaf_usb_demo/dsp/*.h
            ${board_root}/${board}/dsp_examples/xaf_usb_demo/*.h
            ${board_root}/${board}/dsp_examples/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/xaf_usb_demo/dsp
             ${board_root}/${board}/dsp_examples/xaf_usb_demo
             ${board_root}/${board}/dsp_examples/xaf_usb_demo/${core_id}
             ${board_root}/${board}/dsp_examples
)

include(${SdkRootDirPath}/${board_root}/${board}/dsp_examples/xaf_usb_demo/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/dsp_examples/${core_id}.cmake)

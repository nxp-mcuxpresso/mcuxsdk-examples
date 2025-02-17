#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hifi_lc3 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/hifi_lc3)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/edgefast_bluetooth_examples/hifi_lc3/hifi4/*.c
            examples/edgefast_bluetooth_examples/hifi_lc3/hifi4/*.h
            examples/_boards/${board}/edgefast_bluetooth_examples/*.h
            examples/_boards/${board}/edgefast_bluetooth_examples/hifi_lc3/*.h
            examples/_boards/${board}/edgefast_bluetooth_examples/hifi_lc3/hifi4/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/edgefast_bluetooth_examples/hifi_lc3
             examples/edgefast_bluetooth_examples/hifi_lc3/hifi4
             examples/_boards/${board}/edgefast_bluetooth_examples/hifi_lc3
             examples/_boards/${board}/edgefast_bluetooth_examples
)

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/hifi_lc3/hifi4/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/edgefast_bluetooth_examples/hifi_lc3/hifi4/hifi4.cmake)

# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coredump_fault LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/coredump_fault)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/coredump_fault/coredump_fault.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS iar
    SOURCES examples/component_examples/coredump_fault/IAR_UsageFault.s
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    TOOLCHAINS armgcc mdk
    SOURCES examples/component_examples/coredump_fault/GCC_UsageFault.S
)


include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/coredump_fault/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

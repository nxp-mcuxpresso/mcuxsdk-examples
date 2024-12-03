# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_m2mcopy_ezhv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ezhv_examples/m2mcopy)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES m2mcopy_ezhv.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples/m2mcopy/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/ezhv_image.bin TOOLCHAINS riscvllvm)

mcux_add_custom_command(
    BUILD_EVENT POST_BUILD
    TOOLCHAINS riscvllvm
    BUILD_COMMAND 
    ${CMAKE_COMMAND} -E copy ${APPLICATION_BINARY_DIR}/ezhv_image.bin ../binary/ezhv_image.bin
)

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_led_blinky_ezhv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ezhv_examples/led_blinky)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES led_blinky_ezhv.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples/led_blinky/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/ezhv_image.bin TOOLCHAINS riscvllvm)

mcux_add_custom_command(
    BUILD_EVENT POST_BUILD
    TOOLCHAINS riscvllvm
    BUILD_COMMAND 
    ${CMAKE_COMMAND} -E copy ${APPLICATION_BINARY_DIR}/ezhv_image.bin ../binary/ezhv_image.bin
)

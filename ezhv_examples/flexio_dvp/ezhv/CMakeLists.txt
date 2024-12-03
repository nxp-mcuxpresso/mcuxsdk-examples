# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_flexio_dvp_ezhv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ezhv_examples/flexio_dvp)

include(${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples/flexio_dvp/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/ezhv_examples/
    SOURCES ezhv_firmware/ezhv_main.c
            ezhv_firmware/ezhv_camera_dvp.c
            ezhv_firmware/ezhv_api.h
            ezhv_firmware/ezhv_para.h
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/ezhv_image.bin TOOLCHAINS riscvllvm)

mcux_add_custom_command(
    BUILD_EVENT POST_BUILD
    TOOLCHAINS riscvllvm
    BUILD_COMMAND 
    ${CMAKE_COMMAND} -E copy ${APPLICATION_BINARY_DIR}/ezhv_image.bin ../binary/ezhv_image.bin
)

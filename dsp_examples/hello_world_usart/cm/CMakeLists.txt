#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_hello_world_usart LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/hello_world_usart)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/hello_world_usart
    SOURCES dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/hello_world_usart
    INCLUDES ./
)

mcux_add_macro(
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
    CC "-DDSP_IMAGE_COPY_TO_RAM=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/hello_world_usart/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mpi_loader_dsp_hello_world_loader LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/mpi_loader/dsp_hello_world)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/hello_world_usart/cm/main.c
    SOURCES examples/_boards/${board}/dsp_examples/hello_world_usart/dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/component_examples/mpi_loader/dsp_hello_world
             examples/_boards/${board}/dsp_examples/hello_world_usart
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/mpi_loader/dsp_hello_world/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/sdk20-app.bin)

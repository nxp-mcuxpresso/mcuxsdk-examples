#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mpi_loader_dsp_hello_world_loader LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/mpi_loader/dsp_hello_world)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/hello_world_usart/cm/main.c
    SOURCES ${board_root}/${board}/dsp_examples/hello_world_usart/dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/component_examples/mpi_loader/dsp_hello_world
             ${board_root}/${board}/dsp_examples/hello_world_usart
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/mpi_loader/dsp_hello_world/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/sdk20-app.bin)

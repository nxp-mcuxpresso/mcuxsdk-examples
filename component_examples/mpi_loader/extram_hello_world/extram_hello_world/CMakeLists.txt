#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mpi_loader_extram_hello_world LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/mpi_loader/extram_hello_world)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/demo_apps/hello_world
    SOURCES hello_world.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/mpi_loader/extram_hello_world/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/extram_hello_world.bin)

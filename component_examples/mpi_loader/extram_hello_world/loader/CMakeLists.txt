#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mpi_loader_extram_hello_world_loader LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/component_examples/mpi_loader/extram_loader)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES components/mpi_loader/example/extram_loader/extram_loader.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/component_examples/mpi_loader/extram_loader
)

mcux_add_configuration(
    CC "-DFSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL=1 -DFSL_SDK_DISABLE_DRIVER_RESET_CONTROL=1"
    CX "-DFSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL=1 -DFSL_SDK_DISABLE_DRIVER_RESET_CONTROL=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/component_examples/mpi_loader/extram_loader/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/extram_loader.bin)

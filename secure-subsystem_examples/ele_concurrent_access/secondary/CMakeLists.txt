#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ele_concurrent_access_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/secure-subsystem_examples/ele_concurrent_access/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main.c
            ../common_definitions.h
)

mcux_add_include(
    INCLUDES .
    INCLUDES .
)

# We bump the stack size a little
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)

mcux_add_configuration(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=1 -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

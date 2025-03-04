#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_dsp_secondary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp)

include(${SdkRootDirPath}/CMakeLists.txt)

# This will point to: ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/reconfig.cmake
# where common board multicore properties are set.
include(${CMAKE_CURRENT_LIST_DIR}/../../config.cmake)
include(${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/${core_id}/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main.c
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp
    SOURCES dsp_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp
    INCLUDES ./
)

mcux_add_macro(
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
    CC "-DDSP_IMAGE_COPY_TO_RAM=1"
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800 \
             __heap_size__=0x2000 \
             __use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib"
)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

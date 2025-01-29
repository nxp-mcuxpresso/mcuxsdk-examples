#
# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_mpu_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_ns/reconfig.cmake OPTIONAL)


mcux_add_source(
    SOURCES main_ns_asm.s
    TOOLCHAINS iar
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES  ${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_s
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_s/nsc_functions.h
)

mcux_add_source(
    SOURCES main_ns.c
            ../FreeRTOSConfig.h
)

mcux_add_include(
    INCLUDES .
             ../
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=6314 \
        --diag_suppress=6329"
)

mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../freertos_mpu_s/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
    TOOLCHAINS armgcc mdk iar
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

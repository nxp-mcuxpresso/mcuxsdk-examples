#
# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_mpu_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_s)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/freertos_examples/freertos_mpu/freertos_mpu_s/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES main_s.c
            ../FreeRTOSConfig.h
)

mcux_add_source(
    TOOLCHAINS iar
    SOURCES main_s_asm.s
)

mcux_add_include(
    INCLUDES .
             ../
)

# For armgcc first create output directory for cmse lib
mcux_add_custom_command(
    BUILD_EVENT PRE_BUILD
    TOOLCHAINS armgcc
    BUILD_COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}
)

mcux_add_iar_configuration(
    CC "--cmse"
    CX "--cmse"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
)

mcux_add_armgcc_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    LD "--import-cmse-lib-out=\"${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o\""
)

mcux_convert_binary(
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.bin
)

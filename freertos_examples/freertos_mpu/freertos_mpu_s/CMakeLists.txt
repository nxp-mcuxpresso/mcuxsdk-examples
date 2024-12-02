#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_mpu_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_s)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_s/reconfig.cmake OPTIONAL)

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

mcux_remove_iar_configuration(
        AS "--cpu=cortex-m33.no_se"
        CC "--cpu=cortex-m33.no_se"
        CX "--cpu=cortex-m33.no_se"
)

mcux_add_iar_configuration(
    AS "--cpu=cortex-m33"
    CC "--cmse --cpu=cortex-m33"
    CX "--cmse --cpu=cortex-m33"
    )

mcux_add_mdk_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    )

mcux_add_armgcc_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    )

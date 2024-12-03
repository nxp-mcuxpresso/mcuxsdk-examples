#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_mpu_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_ns/reconfig.cmake OPTIONAL)


mcux_add_source(
    SOURCES main_ns_asm.s
    TOOLCHAINS iar
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES  examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_s
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/freertos_examples/freertos_mpu/freertos_mpu_s/nsc_functions.h
)

mcux_add_source(
    SOURCES main_ns.c
            ../FreeRTOSConfig.h
)

mcux_add_include(
    INCLUDES .
             ../
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

#mcux_add_iar_configuration(
#        LD "--image_input=${APPLICATION_BINARY_DIR}/../freertos_mpu/freertos_mpu_s/${MCUX_SDK_PROJECT_NAME}.bin"
#)

mcux_remove_iar_configuration(
    AS "--cpu=cortex-m33.no_se"
    CC "--cpu=cortex-m33.no_se"
    CX "--cpu=cortex-m33.no_se"
    )

mcux_add_iar_configuration(
    AS "--cpu=cortex-m33"
    CC "--cpu=cortex-m33"
    CX "--cpu=cortex-m33"
    )

mcux_add_armgcc_configuration(
    AS "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard"
    CC "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard"
    CX "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard"
    )

mcux_add_mdk_configuration(
    LD "--diag_suppress=6314\
        --diag_suppress=6329"
)

#mcux_add_mcux_configuration(
#    AS "-nonsecure"
#    CC "-nonsecure"
#    CX "-nonsecure"
#    )

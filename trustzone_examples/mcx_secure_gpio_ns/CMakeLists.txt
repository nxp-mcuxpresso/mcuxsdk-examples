#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcx_secure_gpio_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_gpio/reconfig.cmake OPTIONAL)
# Include reconfigure for Non Secure Project
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_gpio/secure_gpio_ns/reconfig.cmake OPTIONAL)

mcux_remove_iar_configuration(
        AS "--fpu=VFPv5_sp"
        CC "--fpu=VFPv5_sp"
        CX "--fpu=VFPv5_sp"
        LD "--fpu=VFPv5_sp"
)

mcux_remove_mdk_configuration(
        AS "-mfpu=fpv5-sp-d16"
        CC "-mfpu=fpv5-sp-d16"
        CX "-mfpu=fpv5-sp-d16"
)

mcux_remove_armgcc_configuration(
        AS "-mfloat-abi=hard -mfpu=fpv5-sp-d16"
        CC "-mfloat-abi=hard -mfpu=fpv5-sp-d16"
        CX "-mfloat-abi=hard -mfpu=fpv5-sp-d16"
        LD "-mfpu=fpv5-sp-d16 -mfloat-abi=hard"
)

mcux_add_source(
    SOURCES secure_gpio_ns.c
            ../mcx_secure_gpio_s/veneer_table.h
)

mcux_add_include(
    INCLUDES ../mcx_secure_gpio_s
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)


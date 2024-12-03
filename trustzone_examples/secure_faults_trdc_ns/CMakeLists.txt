#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(secure_faults_trdc_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/trustzone_examples/secure_faults_trdc/secure_faults_trdc_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
# Include reconfigure for Non Secure Project
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_faults_trdc/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_faults_trdc/secure_faults_trdc_ns/reconfig.cmake OPTIONAL)

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
    SOURCES secure_faults_trdc_ns.c
            ../secure_faults_trdc_s/veneer_table.h
)

mcux_add_include(
    INCLUDES ../secure_faults_trdc_s
)

mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../secure_faults_trdc_s_CMSE_lib.o"
    TOOLCHAINS armgcc mdk iar
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

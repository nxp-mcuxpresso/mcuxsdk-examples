#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(secure_faults_trdc_s LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/trustzone_examples/secure_faults_trdc/secure_faults_trdc_s)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_faults_trdc/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_boards/${board}/trustzone_examples/secure_faults_trdc/secure_faults_trdc_s/reconfig.cmake OPTIONAL)

mcux_add_source(
    SOURCES secure_faults_trdc_s.c
             veneer_table.c
             veneer_table.h
             ../tzm_api/tzm_api.c
             ../tzm_api/tzm_api.h
)

mcux_add_include(
    INCLUDES .
            ../tzm_api
)

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

mcux_add_iar_configuration(
    CC "--cmse"
    CX "--cmse"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/../secure_faults_trdc_s_CMSE_lib.o"
)

mcux_add_armgcc_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/../secure_faults_trdc_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    AS "-mcmse"
    CC "-mcmse"
    CX "-mcmse"
    LD "--import-cmse-lib-out=\"${APPLICATION_BINARY_DIR}/../secure_faults_trdc_s_CMSE_lib.o\""
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

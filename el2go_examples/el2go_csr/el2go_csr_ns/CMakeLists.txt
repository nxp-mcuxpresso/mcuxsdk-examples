#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_csr_ns LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr_ns)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr_ns/reconfig.cmake)

# ------- Define base path variable -------
set(EL2GO_CSR_BASE middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr)

# ------- Select mbedtls compat shim (default: v3, boards targeting 4.x set this in reconfig.cmake) -------
if(NOT DEFINED EL2GO_CSR_MBEDTLS_VERSION)
    set(EL2GO_CSR_MBEDTLS_VERSION "3")
endif()

if(EL2GO_CSR_MBEDTLS_VERSION STREQUAL "4")
    set(_csr_compat_src ${EL2GO_CSR_BASE}/lib/mbedtls_compat/src/csr_mbedtls_compat_v4.c)
    message(STATUS "csr_mbedtls_compat: using Mbed TLS v4 implementation")
else()
    set(_csr_compat_src ${EL2GO_CSR_BASE}/lib/mbedtls_compat/src/csr_mbedtls_compat_v3.c)
    message(STATUS "csr_mbedtls_compat: using Mbed TLS v3 implementation")
endif()

# ------- OS configs ------- (default: baremetal)
set(EL2GO_CSR_OS "baremetal" CACHE STRING "Target OS (baremetal, zephyr)")
set_property(CACHE EL2GO_CSR_OS PROPERTY STRINGS "baremetal" "zephyr")

# ------- el2go csr related configs --------
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${EL2GO_CSR_BASE}/osal/inc/*.h
            ${EL2GO_CSR_BASE}/osal/${EL2GO_CSR_OS}/inc/*.h
            ${EL2GO_CSR_BASE}/osal/${EL2GO_CSR_OS}/src/*.c

            ${EL2GO_CSR_BASE}/pal/inc/*.h
            ${EL2GO_CSR_BASE}/pal/boards/${board}/inc/*.h
            ${EL2GO_CSR_BASE}/pal/boards/${board}/*.c
            
            ${EL2GO_CSR_BASE}/port/*.h

            ${EL2GO_CSR_BASE}/common/inc/*.h
            ${EL2GO_CSR_BASE}/common/src/*.c

            ${EL2GO_CSR_BASE}/app/inc/*.h
            ${EL2GO_CSR_BASE}/app/src/el2go_csr_tlv_parser.c
            ${EL2GO_CSR_BASE}/app/src/el2go_csr.c 

            ${EL2GO_CSR_BASE}/lib/mbedtls_compat/inc/*.h
            ${_csr_compat_src}
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${EL2GO_CSR_BASE}/osal/${EL2GO_CSR_OS}/inc
             ${EL2GO_CSR_BASE}/osal/inc

             ${EL2GO_CSR_BASE}/pal/inc
             ${EL2GO_CSR_BASE}/pal/boards/${board}/inc
            
             ${EL2GO_CSR_BASE}/port

             ${EL2GO_CSR_BASE}/common/inc
             ${EL2GO_CSR_BASE}/app/inc

             ${EL2GO_CSR_BASE}/lib/mbedtls_compat/inc
)

mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1\
        MBEDTLS_USER_CONFIG_FILE=\\\"el2go_csr_mbedtls_user_config.h\\\""
)

# TF-M linker file preprocessing
mcux_add_custom_command(
    TOOLCHAINS armgcc
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc -D${device}${core_id_suffix_name}_SERIES ${TFM_LINKER_DEFINES_IMPORT} -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns.ld
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_custom_command(
    TOOLCHAINS iar
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} -D${device}${core_id_suffix_name}_SERIES ${TFM_LINKER_DEFINES_IMPORT} --cpu=Cortex-M33
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns.icf
    --silent --preprocess=ns ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)

mcux_add_custom_command(
    TOOLCHAINS mdk
    BUILD_EVENT PRE_BUILD
    BUILD_COMMAND ${CC} -D${device}${core_id_suffix_name}_SERIES ${TFM_LINKER_DEFINES_IMPORT} --target=arm-arm-none-eabi -march=armv8-m.main -E -P -xc
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/target/nxp/${board}/partition
    -I ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common
    ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns.sct
    -o ${SdkRootDirPath}/middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct
)

# Add preprocessed TF-M linker files
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/gcc/tfm_common_ns_pre.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/iar/tfm_common_ns_pre.icf
)

mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER middleware/tfm/tf-m/platform/ext/common/armclang/tfm_common_ns_pre.sct
)

# Add TF-M S import library
mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../el2go_csr_s/${CONFIG_TOOLCHAIN}/el2go_csr_s${core_id_suffix_name}_CMSE_lib.o"
    TOOLCHAINS armgcc iar mdk
    GENERATED TRUE
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
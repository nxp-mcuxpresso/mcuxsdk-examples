# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_csr LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr/reconfig.cmake)

# ------- Define base path variable -------
set(EL2GO_CSR_BASE middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr)

#-------- add mbedtls configs -----------
mcux_remove_macro("-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_psa_crypto_config.h\\\"")
mcux_add_macro(
    "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_config.h\\\""
)

# ------- littlefs related configs --------
mcux_add_configuration(
    CC "-DLFS_NO_INTRINSICS=1\
        -DSDK_DEBUGCONSOLE_UART\
        -DLFS_NO_ERROR"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

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
             
)

mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1\
        MBEDTLS_USER_CONFIG_FILE=\\\"el2go_csr_mbedtls_user_config.h\\\""
)

# -------- add executable target -----------
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

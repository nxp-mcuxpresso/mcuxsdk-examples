# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_csr LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/el2go_csr/el2go_csr/reconfig.cmake)

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
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

# ------- OS configs ------- (default: baremetal)
set(EL2GO_CSR_OS "baremetal" CACHE STRING "Target OS (baremetal, zephyr)")
set_property(CACHE EL2GO_CSR_OS PROPERTY STRINGS "baremetal" "zephyr")

# ------- el2go csr related configs --------
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/osal/inc/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/osal/${EL2GO_CSR_OS}/inc/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/osal/${EL2GO_CSR_OS}/src/*.c
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/common/inc/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/common/src/*.c
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/pal/inc/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/pal/boards/${board}/*.c 
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/app/inc/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/app/src/el2go_csr_tlv_parser.c
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/app/src/el2go_csr.c # SPSDK mode
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/port/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/osal/${EL2GO_CSR_OS}/inc
             middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/osal/inc
             middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/pal/inc
             middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/common/inc
             middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/app/inc
             middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_csr/port
)

mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1\
        MBEDTLS_USER_CONFIG_FILE=\\\"el2go_csr_mbedtls_user_config.h\\\""
)

# -------- add executable target -----------
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

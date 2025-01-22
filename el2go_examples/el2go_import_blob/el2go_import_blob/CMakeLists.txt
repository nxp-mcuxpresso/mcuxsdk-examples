# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.30.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(el2go_import_blob LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/el2go_examples/el2go_import_blob/el2go_import_blob)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/reconfig.cmake OPTIONAL)
include(${SdkRootDirPath}/${board_root}/${board}/el2go_examples/el2go_import_blob/el2go_import_blob/reconfig.cmake OPTIONAL)

# ------- littlefs related configs --------
mcux_add_configuration(
    CC "-DLFS_NO_INTRINSICS=1\
        -DSDK_DEBUGCONSOLE_UART"
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS release
    CC "-DLFS_NO_ASSERT"
)

# ------- el2go import blob related configs --------
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/*.h
            middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob/*.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/nxp_iot_agent/ex/src/apps/psa_examples/el2go_import_blob
)

mcux_add_macro(
    CC "PRINTF_ADVANCED_ENABLE=1"
)

mcux_add_iar_configuration(
    CC "--diag_suppress=Pe546,Pe111,Pe177"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x3000\
             __heap_size__=0x8000"
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
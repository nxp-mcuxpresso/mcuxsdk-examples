#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rstp_example LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/netc/rstp_example)

include(${SdkRootDirPath}/CMakeLists.txt)

# Add main application sources
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
        examples/driver_examples/netc/rstp_example/main.c
        examples/driver_examples/netc/rstp_example/netc_switch.c
        examples/driver_examples/netc/rstp_example/stp_task.c
        examples/driver_examples/netc/rstp_example/stp_freertos_adapter.c
)

# Add include paths
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES 
        ${board_root}/${board}/driver_examples/netc/rstp_example
        ${board_root}/${board}/driver_examples/netc/rstp_example/${core_id}
)

# C++ compiler flags
mcux_add_iar_configuration(
    CX "--diag_suppress Pa082,Pa050,Pe161"
)

# C/C++ compiler macros
mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_NETC_USED=1\
       -DSTP_USE_LOG=1"
)

# Linker configurations for different toolchains
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x8000"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x8000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)

# Include optional reconfiguration file
include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/netc/rstp_example/reconfig.cmake OPTIONAL)

# Convert to binary format
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)


#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_prp LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/freertos_examples/freertos_prp)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES freertos_prp.c
            prp_device.c
            prp_device.h
            prp_example.c
            prp_forward.c
            prp_forward.h
            prp_frame.c
            prp_frame.h
            prp_node.c
            prp_node.h
            prp_supervision.c
            prp_types.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_macro(
  CC "-DSDK_NETC_USED=1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000\
        -Xlinker --defsym=__heap_size__=0x4000"
)

include(${SdkRootDirPath}/${board_root}/${board}/freertos_examples/freertos_prp/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

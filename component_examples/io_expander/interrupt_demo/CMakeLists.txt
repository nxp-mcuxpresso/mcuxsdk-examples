# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(io_expander_interrupt_demo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/io_expander/interrupt_demo/)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/component_examples/io_expander/interrupt_demo/interrupt_demo.c
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/io_expander/interrupt_demo/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

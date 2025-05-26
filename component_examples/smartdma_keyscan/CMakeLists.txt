# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(smartdma_keyscan LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/smartdma_keyscan)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/component_examples/smartdma_keyscan
             ${board_root}/${board}/component_examples/smartdma_keyscan/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/smartdma_keyscan/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
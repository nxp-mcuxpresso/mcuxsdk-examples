# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpadc_dma3 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/lpadc/dma3)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpadc/dma3/lpadc_dma3.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lpadc/dma3
             ${board_root}/${board}/driver_examples/lpadc/dma3/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/lpadc/dma3/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

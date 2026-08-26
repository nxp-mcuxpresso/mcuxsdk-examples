# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(v2x_fce_aes_gcm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/v2x_fce_examples/v2x_fce_aes_gcm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(SOURCES v2x_fce_aes_gcm.c)
mcux_add_include(INCLUDES .)


include(${SdkRootDirPath}/${board_root}/${board}/v2x_fce_examples/v2x_fce_aes_gcm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

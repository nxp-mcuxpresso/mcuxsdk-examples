#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_mu_polling LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/mu_polling)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES main.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/dsp_examples/${core_id}.cmake)


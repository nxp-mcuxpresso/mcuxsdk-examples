#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_naturedsp LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/naturedsp)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES *.c
            *.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/naturedsp/${core_id}
    SOURCES   naturedsp_input.h
)


mcux_add_include(
    INCLUDES .
             ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/naturedsp/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/naturedsp/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/dsp_examples/${core_id}.cmake)

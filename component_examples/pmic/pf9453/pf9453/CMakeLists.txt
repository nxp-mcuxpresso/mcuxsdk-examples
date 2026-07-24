# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(pf9453 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/component_examples/pf9453)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/component_examples/pmic/pf9453/pf9453/pf9453.c
)

include(${SdkRootDirPath}/${board_root}/${board}/component_examples/pf9453/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_with_hifi_primary LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_mode_with_hifi/)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_with_hifi/reconfig.cmake OPTIONAL)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_mode_switch_secondary/${CONFIG_TOOLCHAIN}/
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
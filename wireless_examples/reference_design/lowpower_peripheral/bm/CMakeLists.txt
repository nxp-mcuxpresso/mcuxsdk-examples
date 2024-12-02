# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

# Here we extend the prj.conf searching path by setting CUSTOM_PRJ_CONF_PATH to support board specific overrides
# relative to an out-of-tree application. A good use case example is the freertos kernel port choice, depending on
# the application you may want to choose between secure/non-secure port, but the port also depends on the platform
# core, so this is where we need to be able to define those overrides.
project(lp_refdes_ble_peripheral_bm LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/reference_design/lowpower_peripheral/lowpower_peripheral.cmake)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_ble_15_4_dyn_mac_split LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        middleware/wireless/ble_controller/src/KW4x
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/ble_controller/nbu_ble
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/ieee-802.15.4/nbu_ble_15_4_dyn/nbu_ble_15_4_dyn.cmake)

mcux_add_macro(MAC_ENABLED)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/nbu_ble_15_4_dyn_mac_split.bin
)

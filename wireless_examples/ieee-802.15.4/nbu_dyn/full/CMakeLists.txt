# Copyright 2025-2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_dyn_full LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        middleware/wireless/ble_controller/src/KW4x
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/ble_controller/nbu_ble
        ${board_root}/${board}/wireless_examples/ieee-802.15.4/nbu_dyn/full
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/ieee-802.15.4/nbu_dyn/nbu_dyn.cmake)
include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/ieee-802.15.4/nbu_dyn/full/reconfig.cmake)

# Copyright 2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)
include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(nbu_ble_15_4_dyn_phy_split_matter LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH
        middleware/wireless/ble_controller/src/KW4x
    CUSTOM_PRJ_CONF_PATH
        ${board_root}/${board}/wireless_examples/ble_controller/nbu_ble
        ${board_root}/${board}/wireless_examples/ieee-802.15.4/nbu_ble_15_4_dyn/phy_split_matter
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/wireless_examples/ieee-802.15.4/nbu_ble_15_4_dyn/nbu_ble_15_4_dyn.cmake)
include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/ieee-802.15.4/nbu_ble_15_4_dyn/phy_split_matter/reconfig.cmake)

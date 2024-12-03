# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(15_4_connectivity_test_bm LANGUAGES C CXX ASM
                PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
                CUSTOM_PRJ_CONF_PATH examples/_boards/${board}/wireless_examples/ieee-802.15.4/connectivity_test/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples/_boards/${board}/wireless_examples/ieee-802.15.4/connectivity_test/bm/reconfig.cmake)

include(../connectivity_test.cmake)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
if(${core_id} STREQUAL cm33)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M33")
elseif(${core_id} STREQUAL cm7)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M7")
elseif(${core_id} STREQUAL cm4)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M4")
elseif(${core_id} STREQUAL cm33_core0)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M33_0")
elseif(${core_id} STREQUAL cm33_core1)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M33_1")
endif()

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-MCXN947")
#board_runner_args(linkserver  "--core=${core_id}")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

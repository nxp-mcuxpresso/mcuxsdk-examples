# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
if(${core_id} STREQUAL cm33_core0)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M33_0")
elseif(${core_id} STREQUAL cm33_core1)
    board_runner_args(jlink "--device=${CONFIG_MCUX_HW_DEVICE_ID}_M33_1")
endif()

board_runner_args(jlink "--device=${CONFIG_MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER}")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:MCX-W72-EVK")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

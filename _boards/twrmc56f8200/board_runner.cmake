# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=TWRMC56F8200")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:TWRMC56F8200")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

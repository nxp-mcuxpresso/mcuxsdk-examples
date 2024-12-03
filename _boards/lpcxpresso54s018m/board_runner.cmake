# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=LPC54S018M")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:LPCXpresso54S018M")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=${CONFIG_MCUX_TOOLCHAIN_JLINK_CPU_IDENTIFIER}")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-MCXW70")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

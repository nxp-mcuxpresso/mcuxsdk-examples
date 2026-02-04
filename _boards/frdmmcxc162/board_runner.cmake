# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MCXC162")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-MCXC162")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

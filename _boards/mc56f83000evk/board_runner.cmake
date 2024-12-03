# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MC56F83000EVK")

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:MC56F83000EVK")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

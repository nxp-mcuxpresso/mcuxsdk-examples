# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MCIMX7U5")
board_runner_args(linkserver  "--device=MCIMX7U5:EVK-MCIMX7ULP")
board_runner_args(pyocd "--target=mcimx7u5")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

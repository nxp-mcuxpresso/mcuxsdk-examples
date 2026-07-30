# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MIMX93W52_M33")
board_runner_args(linkserver  "--device=MIMX93W52:MCIMX93W-EVK")
board_runner_args(pyocd "--target=mimx93W52")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

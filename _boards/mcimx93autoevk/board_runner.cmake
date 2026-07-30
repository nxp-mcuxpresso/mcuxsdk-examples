# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MIMX9352_M33")
board_runner_args(linkserver  "--device=MIMX9352:MCIMX93AUTO-EVK")
board_runner_args(pyocd "--target=mimx9352")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

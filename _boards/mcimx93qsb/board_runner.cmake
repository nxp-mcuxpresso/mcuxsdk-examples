# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MIMX9322")
board_runner_args(linkserver  "--device=MIMX9322:MCIMX93-QSB")
board_runner_args(pyocd "--target=mimx9322")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

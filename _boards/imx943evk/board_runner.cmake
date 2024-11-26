# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
board_runner_args(jlink "--device=MIMX94398")
board_runner_args(linkserver  "--device=MIMX94398:IMX943-EVK")
board_runner_args(pyocd "--target=imx943")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

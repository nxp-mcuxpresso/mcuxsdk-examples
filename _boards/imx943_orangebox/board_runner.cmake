# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
if(${core_id} STREQUAL cm33_core0)
board_runner_args(jlink "--device=IMX943_M33_0")
elseif(${core_id} STREQUAL cm33_core1)
board_runner_args(jlink "--device=IMX943_M33_1")
elseif(${core_id} STREQUAL cm7_core0)
board_runner_args(jlink "--device=IMX943_M7_0")
elseif(${core_id} STREQUAL cm7_core1)
board_runner_args(jlink "--device=IMX943_M7_1")
endif()
board_runner_args(linkserver  "--device=MIMX94398:IMX943-ORANGEBOX")
board_runner_args(pyocd "--target=imx943")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/pyocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/openocd.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/canopen.board.cmake)

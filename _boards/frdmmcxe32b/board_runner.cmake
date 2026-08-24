# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
if(${core_id} STREQUAL core0)
	board_runner_args(jlink "--device=MCXE32B_M7_0")
	board_runner_args(linkserver "--core=core0")
elseif(${core_id} STREQUAL core1)
	board_runner_args(jlink "--device=MCXE32B_M7_1")
	board_runner_args(linkserver "--core=core1")
endif()

board_runner_args(linkserver  "--device=${CONFIG_MCUX_HW_DEVICE_ID}:FRDM-MCXE32B")

include(${SdkRootDirPath}/cmake/extension/runner/linkserver.board.cmake)
include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)

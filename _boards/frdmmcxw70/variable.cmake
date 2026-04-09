# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmmcxw70)
mcux_set_variable(board_root examples/_boards)
mcux_set_variable(bluetooth_root bluetooth)

if (NOT DEFINED device)
    mcux_set_variable(device MCXW70AC)
endif()

include(${SdkRootDirPath}/devices/MCX/MCXW/${device}/variable.cmake)

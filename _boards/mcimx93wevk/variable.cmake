# Copyright 2025 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mcimx93wevk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX93W52)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX93W/${device}/variable.cmake)

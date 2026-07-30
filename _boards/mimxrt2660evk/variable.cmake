# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mimxrt2660evk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT2663)
endif()

include(${SdkRootDirPath}/devices/RT/RT2660/${device}/variable.cmake)

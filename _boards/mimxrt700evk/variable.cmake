# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mimxrt700evk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT798S)
endif()

include(${SdkRootDirPath}/devices/RT/RT700/${device}/variable.cmake)

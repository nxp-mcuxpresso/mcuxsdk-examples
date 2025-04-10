# Copyright 2024-2025 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mc56f83000evk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MC56F83789)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MC56F83xxx)
endif()

include(${SdkRootDirPath}/devices/DSC/${soc_series}/${device}/variable.cmake)

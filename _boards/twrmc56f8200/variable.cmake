# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board twrmc56f8200)

if (NOT DEFINED device)
    mcux_set_variable(device MC56F82748)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MC56F82xxx)
endif()

include(${SdkRootDirPath}/devices/DSC/${soc_series}/${device}/variable.cmake)

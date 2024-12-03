# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mcimx93qsb)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX9322)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX93/${device}/variable.cmake)

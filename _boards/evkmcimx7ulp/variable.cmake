# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmcimx7ulp)

if (NOT DEFINED device)
    mcux_set_variable(device MCIMX7U5)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX7ULP/${device}/variable.cmake)

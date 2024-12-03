# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmimx8ulp)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX8UD7)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX8ULP/${device}/variable.cmake)

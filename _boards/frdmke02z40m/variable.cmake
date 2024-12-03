# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmke02z40m)

if (NOT DEFINED device)
    mcux_set_variable(device MKE02Z4)
endif()

include(${SdkRootDirPath}/devices/Kinetis/KE/${device}/variable.cmake)

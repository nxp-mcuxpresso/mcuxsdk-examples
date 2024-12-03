# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmk32l3a6)

if (NOT DEFINED device)
    mcux_set_variable(device K32L3A60)
endif()

include(${SdkRootDirPath}/devices/Kinetis/K32L/${device}/variable.cmake)

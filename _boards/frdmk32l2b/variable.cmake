# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmk32l2b)

if (NOT DEFINED device)
    mcux_set_variable(device K32L2B31A)
endif()

include(${SdkRootDirPath}/devices/Kinetis/K32L/${device}/variable.cmake)

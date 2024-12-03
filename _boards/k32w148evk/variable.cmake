# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board k32w148evk)

if (NOT DEFINED device)
    mcux_set_variable(device K32W1480)
endif()

include(${SdkRootDirPath}/devices/Wireless/K32W/${device}/variable.cmake)

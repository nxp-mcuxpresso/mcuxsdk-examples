# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mcxw72evk)

if (NOT DEFINED device)
    mcux_set_variable(device MCXW727C)
endif()

include(${SdkRootDirPath}/devices/MCX/MCXW/${device}/variable.cmake)

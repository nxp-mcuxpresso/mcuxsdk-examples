# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board kw47evk)

if (NOT DEFINED device)
    mcux_set_variable(device KW47B42ZB7)
endif()

include(${SdkRootDirPath}/devices/Wireless/KW/${device}/variable.cmake)

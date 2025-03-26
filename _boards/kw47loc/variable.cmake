# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board kw47loc)
mcux_set_variable(board_root examples/_boards)
mcux_set_variable(bluetooth_root bluetooth)

if (NOT DEFINED device)
    mcux_set_variable(device KW47B42ZB7)
endif()

include(${SdkRootDirPath}/devices/Wireless/KW/${device}/variable.cmake)

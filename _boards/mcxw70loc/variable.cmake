# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mcxw70loc)
mcux_set_variable(board_root examples/_boards)
mcux_set_variable(bluetooth_root bluetooth)

if (NOT DEFINED device)
    mcux_set_variable(device KW43B43ZC7)
endif()

include(${SdkRootDirPath}/devices/Wireless/KW/${device}/variable.cmake)

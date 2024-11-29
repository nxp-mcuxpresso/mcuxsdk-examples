# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmke17z)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MKE17Z7)
endif()

include(${SdkRootDirPath}/devices/Kinetis/KE/${device}/variable.cmake)

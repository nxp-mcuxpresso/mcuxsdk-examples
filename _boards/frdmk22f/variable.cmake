# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmk22f)

if (NOT DEFINED device)
    mcux_set_variable(device MK22F51212)
endif()

include(${SdkRootDirPath}/devices/Kinetis/K/${device}/variable.cmake)

# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board twrkm34z50mv3)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MKM34ZA5)
endif()

include(${SdkRootDirPath}/devices/Kinetis/KM/${device}/variable.cmake)

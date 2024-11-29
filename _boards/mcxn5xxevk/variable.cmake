# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mcxn5xxevk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MCXN547)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MCXN)
endif()

include(${SdkRootDirPath}/devices/MCX/${soc_series}/${device}/variable.cmake)

# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmmcxw23)
mcux_set_variable(board_root examples/_boards)
mcux_set_variable(bluetooth_root bluetooth)

if (NOT DEFINED device)
    mcux_set_variable(device MCXW236)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MCXW)
endif()

include(${SdkRootDirPath}/devices/MCX/${soc_series}/${device}/variable.cmake)

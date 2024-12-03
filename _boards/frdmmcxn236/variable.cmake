# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmmcxn236)

if (NOT DEFINED device)
    mcux_set_variable(device MCXN236)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MCXN)
endif()

include(${SdkRootDirPath}/devices/MCX/${soc_series}/${device}/variable.cmake)

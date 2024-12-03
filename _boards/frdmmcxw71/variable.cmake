# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmmcxw71)

if (NOT DEFINED device)
    mcux_set_variable(device MCXW716C)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MCXW)
endif()

include(${SdkRootDirPath}/devices/MCX/${soc_series}/${device}/variable.cmake)

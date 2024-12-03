# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmmcxa153)

if (NOT DEFINED device)
    mcux_set_variable(device MCXA153)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series MCXA)
endif()

include(${SdkRootDirPath}/devices/MCX/${soc_series}/${device}/variable.cmake)

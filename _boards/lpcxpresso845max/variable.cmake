# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board lpcxpresso845max)

if (NOT DEFINED device)
    mcux_set_variable(device LPC845)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series LPC800)
endif()

include(${SdkRootDirPath}/devices/LPC/${soc_series}/${device}/variable.cmake)

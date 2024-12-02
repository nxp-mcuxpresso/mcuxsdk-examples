# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board lpcxpresso54s018)

if (NOT DEFINED device)
    mcux_set_variable(device LPC54S018)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series LPC54000)
endif()

include(${SdkRootDirPath}/devices/LPC/${soc_series}/${device}/variable.cmake)

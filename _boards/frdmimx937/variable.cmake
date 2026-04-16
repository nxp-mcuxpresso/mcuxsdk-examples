# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmimx937)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX9375)
endif()

mcux_set_variable(soc_series i.MX937)

include(${SdkRootDirPath}/devices/i.MX/${soc_series}/${device}/variable.cmake)

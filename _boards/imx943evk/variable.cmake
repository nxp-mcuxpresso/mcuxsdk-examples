# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board imx943evk)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX94398)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX943/${device}/variable.cmake)

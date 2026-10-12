# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board imxrtl345mb)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device IMXRTL3459AAA)
endif()

if (NOT DEFINED soc_series)
    mcux_set_variable(soc_series RTL345)
endif()

include(${SdkRootDirPath}/devices/RT/${soc_series}/${device}/variable.cmake)

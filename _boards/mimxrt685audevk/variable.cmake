# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board mimxrt685audevk)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT685S)
endif()

include(${SdkRootDirPath}/devices/RT/RT600/${device}/variable.cmake)

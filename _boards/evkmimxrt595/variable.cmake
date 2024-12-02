# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmimxrt595)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT595S)
endif()

include(${SdkRootDirPath}/devices/RT/RT500/${device}/variable.cmake)

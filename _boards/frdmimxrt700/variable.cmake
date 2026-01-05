# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmimxrt700)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT798S)
    message(STATUS "!!!!!!!...")
endif()

include(${SdkRootDirPath}/devices/RT/RT700/${device}/variable.cmake)

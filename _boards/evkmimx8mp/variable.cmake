# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmimx8mp)
mcux_set_variable(board_root examples/_boards)

# Remove the section '.stacktop_and_pc'
mcux_set_variable(__EXTRA_ARGS "--remove-section=.stacktop_and_pc")

if (NOT DEFINED device)
    mcux_set_variable(device MIMX8ML8)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX8MP/${device}/variable.cmake)

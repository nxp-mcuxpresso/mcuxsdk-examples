# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmimx8mnddr3l)
mcux_set_variable(board_root examples/_boards)

if (CONFIG_TOOLCHAIN STREQUAL "armgcc")
# Remove the section '.stacktop_and_pc'
mcux_set_variable(__EXTRA_ARGS "--remove-section=.stacktop_and_pc")
endif()

if (NOT DEFINED device)
    mcux_set_variable(device MIMX8MN6)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX8MN/${device}/variable.cmake)

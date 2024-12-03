# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board kw45b41zevk)

if (NOT DEFINED device)
    mcux_set_variable(device KW45B41Z83)
endif()

include(${SdkRootDirPath}/devices/Wireless/KW/${device}/variable.cmake)

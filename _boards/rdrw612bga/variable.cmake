# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board rdrw612bga)

if (NOT DEFINED device)
    mcux_set_variable(device RW612)
endif()

include(${SdkRootDirPath}/devices/Wireless/RW/${device}/variable.cmake)

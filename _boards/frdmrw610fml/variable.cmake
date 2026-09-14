# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board frdmrw610fml)
mcux_set_variable(board_root examples/_boards)

if (NOT DEFINED device)
    mcux_set_variable(device RW610)
endif()

include(${SdkRootDirPath}/devices/Wireless/RW/${device}/variable.cmake)
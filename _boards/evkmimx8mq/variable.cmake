# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_set_variable(board evkmimx8mq)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX8MQ6)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX8M/${device}/variable.cmake)

# Copyright 2026 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

if(core_id STREQUAL "cm0plus")
    mcux_add_armgcc_configuration(
        CC "-O1"
        CX "-O1"
    )
endif()

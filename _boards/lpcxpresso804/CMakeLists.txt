# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)

if (CONFIG_MCUX_PRJSEG_config.arm.core.cm0p)
    mcux_add_mdk_configuration(
            LD "--keep=*(.crp)"
    )
endif()

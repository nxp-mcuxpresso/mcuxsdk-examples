# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#

include(${SdkRootDirPath}/arch/arm/target/flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_config.arm.core.cm0p)
    mcux_add_mdk_configuration(
            LD "--keep=*(.crp)"
    )
endif()

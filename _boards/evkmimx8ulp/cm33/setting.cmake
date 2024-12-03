# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/dsp/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        CC "-DEVK_MIMX8ULP -DCPU_MIMX8UD7DVP08_cm33"
    )
endif()


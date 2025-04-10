# Copyright 2024-2025 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/dsp56800/target/flash_sdm_lpm.cmake)
include(${SdkRootDirPath}/arch/dsp56800/target/flash_ldm_lpm.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/dsp56800_common.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_codewarrior_configuration(
            CC "-DPRINTF_ADVANCED_ENABLE=1"
    )

endif()

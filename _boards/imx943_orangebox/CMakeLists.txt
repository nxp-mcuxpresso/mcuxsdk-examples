# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_boards/imx943_orangebox/setting.cmake)
include(${SdkRootDirPath}/examples/_boards/imx943_orangebox/${core_id}/setting.cmake)
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/ddr.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
endif()

if (CONFIG_MCUX_COMPONENT_driver.srtm)
    mcux_add_source(
        SOURCES
            rsc_table.c
            rsc_table.h
    )

    mcux_add_include(
        INCLUDES
            .
    )

    mcux_add_configuration(
            CC "-DSCMI_LMM_POWER_CHANGE_PROCESSED=1"
    )
endif()

if (CONFIG_MCUX_COMPONENT_driver.srtm_config)

    mcux_add_source(
        SOURCES
            srtm_config.h
    )

    mcux_add_include(
        INCLUDES
            .
    )
endif()

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_boards/imx943evk/setting.cmake)
include(${SdkRootDirPath}/examples/_boards/imx943evk/${core_id}/setting.cmake)
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/ddr.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
endif()

if (CONFIG_MCUX_COMPONENT_driver.sm_cfg)
    # common sm hal configuration
    mcux_add_source(
        SOURCES sm/hal_config.h
    )

    mcux_add_include(
        INCLUDES sm/
    )

    mcux_add_configuration(
            CC "-DFSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL=1"
    )
endif()

if (CONFIG_MCUX_COMPONENT_driver.non_sm_cfg)
    # common non sm hal configuration
    mcux_add_source(
        SOURCES non_sm/hal_config.h
    )

    mcux_add_include(
        INCLUDES non_sm/
    )

endif()


if (CONFIG_MCUX_COMPONENT_driver.srtm)

    mcux_add_source(
        SOURCES
            ./${core_id}/app_srtm.c
            rsc_table.c
    )

    mcux_add_include(
        INCLUDES
            ./${core_id}
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.rpmsg_config)

    mcux_add_source(
        SOURCES
            ./${core_id}/rpmsg_config.h
    )

    mcux_add_include(
        INCLUDES 
            ./${core_id}
    )

endif()

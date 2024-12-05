# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/ext_flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/examples/_boards/imx95verdinevk/${core_id}/setting.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)

# common sm hal configuration
if (CONFIG_MCUX_PRJSEG_module.board.suite)
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

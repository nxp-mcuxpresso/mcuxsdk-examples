# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/ddr.cmake)
include(${SdkRootDirPath}/arch/arm/target/ext_flash.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if(CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES ${board_root}/evkmimx8mn/clock_config.h ${board_root}/evkmimx8mn/clock_config.c)
    mcux_add_source(BASE_PATH ${SdkRootDirPath} SOURCES ${board_root}/evkmimx8mn/board.h ${board_root}/evkmimx8mn/board.c)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${board_root}/evkmimx8mn)

    mcux_add_macro(
	CC "-DVDEV0_VRING_BASE=0x78000000U"
    )
endif()

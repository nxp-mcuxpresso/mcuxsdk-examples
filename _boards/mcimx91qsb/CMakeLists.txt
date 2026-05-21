# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

include(${SdkRootDirPath}/arch/arm/target/ddr.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/common_freertos/common_freertos.cmake)

mcux_add_source(
    SOURCES
        ./mmu.c
)

mcux_add_include(
    INCLUDES
        ./
)

# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.suite)
mcux_add_source(
    SOURCES
        ./board.c
        ./mmu.c
)

mcux_add_include(
    INCLUDES
        ./
)
endif()


include(${CMAKE_CURRENT_LIST_DIR}/common_freertos/common_freertos.cmake)


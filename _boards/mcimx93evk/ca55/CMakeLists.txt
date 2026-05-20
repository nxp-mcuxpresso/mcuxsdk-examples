# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES
        ./mmu.c
)

mcux_add_include(
    INCLUDES
        ./
)

include(${CMAKE_CURRENT_LIST_DIR}/common_freertos/common_freertos.cmake)


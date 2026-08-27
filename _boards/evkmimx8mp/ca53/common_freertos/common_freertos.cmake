# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES
    FreeRTOSConfig.h
    FreeRTOS_helper.c
    FreeRTOS_tick_config.c
    idle.c
    irq.c
)

mcux_add_include(
    INCLUDES
    .
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

mcux_add_source(
    SOURCES
        SOURCES ./FreeRTOSConfig.h
)

mcux_add_include(
    INCLUDES ./
)

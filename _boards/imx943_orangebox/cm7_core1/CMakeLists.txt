# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_driver.app_srtm.cm7_core1)
    mcux_add_source(
        SOURCES
            ./app_srtm.c
            ./app_srtm.h
    )

    mcux_add_include(
        INCLUDES
            ./
    )

endif()

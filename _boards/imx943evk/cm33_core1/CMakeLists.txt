# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_COMPONENT_driver.app_srtm.cm33_core1)
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

if (CONFIG_MCUX_COMPONENT_driver.rpmsg_config.cm33_core1)

    mcux_add_source(
        SOURCES
            ./rpmsg_config.h
    )

    mcux_add_include(
        INCLUDES 
            ./
    )

endif()



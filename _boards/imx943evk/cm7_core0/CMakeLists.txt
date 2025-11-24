# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.app_srtm.cm7_core0)
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

if (CONFIG_MCUX_PRJSEG_module.rpmsg_config.cm7_core0)

    mcux_add_source(
        SOURCES
            ./rpmsg_config.h
    )

    mcux_add_include(
        INCLUDES 
            ./
    )

endif()



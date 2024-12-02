# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.shield.a8974.common)
    mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES examples/_boards/mimxrt700evk/shields/a8974)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/mimxrt700evk/shields/a8974
        SOURCES 
            mimxrt700evk.h 
            RTE_Device.h
            issdk_hal.h
            frdm_stbi_a8974_shield.h
            mimxrt700evk.c
    )
endif()
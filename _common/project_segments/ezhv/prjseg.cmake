# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.ezhv_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples
        SOURCES ezhv_support.c
                ezhv_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples
        INCLUDES ./
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.incbin.ezhv)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples
        SOURCES incbin_ezhv.S
        TOOLCHAINS mcux mdk armgcc
    )
endif()

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.ce_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ce_examples
        SOURCES ce_support.c
                ce_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/ce_examples
        INCLUDES ./
    )
endif()

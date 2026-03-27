# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_PRJSEG_module.ce_lite_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/ce_lite_examples
        SOURCES ce_support.c
                ce_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/ce_lite_examples
        INCLUDES ./
    )
endif()

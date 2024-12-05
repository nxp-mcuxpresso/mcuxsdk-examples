# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.vglite_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/vglite_examples/vglite_support/vglite_support.h
                ${board_root}/${board}/vglite_examples/vglite_support/vglite_support.c
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}/vglite_examples/vglite_support
    )
endif()

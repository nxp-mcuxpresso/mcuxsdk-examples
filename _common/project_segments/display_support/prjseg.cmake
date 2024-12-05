# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES ${board_root}/${board}/display_support.h
                ${board_root}/${board}/display_support.c
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES ${board_root}/${board}
    )
endif()

include(${SdkRootDirPath}/${board_root}/${board}/project_segments/display_support/prjseg.cmake OPTIONAL)

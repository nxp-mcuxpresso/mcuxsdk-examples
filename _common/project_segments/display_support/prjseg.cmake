# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.display_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/_boards/${board}/display_support.h
                examples/_boards/${board}/display_support.c
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/_boards/${board}
    )
endif()

include(${SdkRootDirPath}/examples/_boards/${board}/project_segments/display_support/prjseg.cmake OPTIONAL)

# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

if (CONFIG_MCUX_PRJSEG_module.board.lvgl)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}
        SOURCES examples/lvgl_examples/lvgl_sdk/lv_conf.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}
        INCLUDES examples/lvgl_examples/lvgl_sdk
    )
endif()

include(${SdkRootDirPath}/${board_root}/${board}/project_segments/lvgl/prjseg.cmake OPTIONAL)

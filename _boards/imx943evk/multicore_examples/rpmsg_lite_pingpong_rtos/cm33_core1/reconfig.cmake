#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DRPMSG_M331_MASTER=1"
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos/${core_id}/rpmsg_config.h
)

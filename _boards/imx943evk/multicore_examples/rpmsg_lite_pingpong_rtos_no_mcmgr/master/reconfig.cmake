#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
mcux_add_include(
    INCLUDES .
    INCLUDES ${core_id}
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_no_mcmgr/master/rpmsg_config.h
)

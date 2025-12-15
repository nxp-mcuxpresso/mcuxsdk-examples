#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/mcimx93wevk/FreeRTOSConfigBoard.h
            ${board_root}/mcimx93wevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            ${board_root}/mcimx93wevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
            ${board_root}/mcimx93wevk/srtm_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/mcimx93wevk
             middleware/multicore/remoteproc
             ${board_root}/mcimx93wevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             ${board_root}/mcimx93wevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)

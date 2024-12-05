#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/mcimx93autoevk/FreeRTOSConfigBoard.h
            ${board_root}/mcimx93autoevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            ${board_root}/mcimx93autoevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
            ${board_root}/mcimx93autoevk/srtm_config.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/mcimx93autoevk
             middleware/multicore/remoteproc
             ${board_root}/mcimx93autoevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             ${board_root}/mcimx93autoevk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/mcimx93evk/FreeRTOSConfigBoard.h
            examples/_boards/mcimx93evk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            examples/_boards/mcimx93evk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
            examples/_boards/mcimx93evk/srtm_config.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/mcimx93evk
             middleware/multicore/remoteproc
             examples/_boards/mcimx93evk/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             examples/_boards/mcimx93evk/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)

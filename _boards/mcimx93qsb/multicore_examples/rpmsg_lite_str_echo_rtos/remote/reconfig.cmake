#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/mcimx93qsb/FreeRTOSConfigBoard.h
            ${board_root}/mcimx93qsb/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
            ${board_root}/mcimx93qsb/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            ${board_root}/mcimx93qsb/srtm_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/mcimx93qsb
             middleware/multicore/remoteproc
             ${board_root}/mcimx93qsb/multicore_examples/rpmsg_lite_str_echo_rtos
             ${board_root}/mcimx93qsb/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)

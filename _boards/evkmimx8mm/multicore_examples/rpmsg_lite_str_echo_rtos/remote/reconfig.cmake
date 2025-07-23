#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/evkmimx8mm/FreeRTOSConfigBoard.h
            ${board_root}/evkmimx8mm/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
	    ${board_root}/evkmimx8mm/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            ${board_root}/evkmimx8mm/srtm_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mm
             middleware/multicore/remoteproc
             ${board_root}/evkmimx8mm/multicore_examples/rpmsg_lite_str_echo_rtos
             ${board_root}/evkmimx8mm/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

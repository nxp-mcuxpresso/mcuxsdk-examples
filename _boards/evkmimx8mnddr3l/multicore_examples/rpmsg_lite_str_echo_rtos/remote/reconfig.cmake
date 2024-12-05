#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/evkmimx8mn/FreeRTOSConfigBoard.h
            ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
	    ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            ${board_root}/evkmimx8mn/srtm_config.h
            ${board_root}/evkmimx8mn/rsc_table.c
            ${board_root}/evkmimx8mn/rsc_table.h
            ${board_root}/evkmimx8mn/srtm_config.h
            ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/pin_mux.h
            ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/pin_mux.c
            ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/app.h
            ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/hardware_init.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn
             middleware/multicore/remoteproc
             ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos
             ${board_root}/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)

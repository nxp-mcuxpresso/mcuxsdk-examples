#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/evkmimx8mn/FreeRTOSConfigBoard.h
            examples/_boards/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
	    examples/_boards/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            examples/_boards/evkmimx8mn/srtm_config.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            examples/_boards/evkmimx8mn/srtm_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn
             middleware/multicore/remoteproc
             examples/_boards/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos
             examples/_boards/evkmimx8mn/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)

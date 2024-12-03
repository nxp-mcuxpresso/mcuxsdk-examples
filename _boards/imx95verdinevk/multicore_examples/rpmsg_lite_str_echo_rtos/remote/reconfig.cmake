#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
            examples/_boards/${board}/srtm_config.h
	    examples/_boards/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/remote/cm7/rpmsg_config.h
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            examples/_boards/${board}/cm7/app_srtm.c
            examples/_boards/${board}/cm7/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/imx95verdinevk
             middleware/multicore/remoteproc
	     examples/_boards/imx95verdinevk/cm7
             examples/_boards/imx95verdinevk/multicore_examples/rpmsg_lite_str_echo_rtos
             examples/_boards/imx95verdinevk/multicore_examples/rpmsg_lite_str_echo_rtos/remote
	     examples/_boards/imx95verdinevk/multicore_examples/rpmsg_lite_str_echo_rtos/remote/cm7
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)


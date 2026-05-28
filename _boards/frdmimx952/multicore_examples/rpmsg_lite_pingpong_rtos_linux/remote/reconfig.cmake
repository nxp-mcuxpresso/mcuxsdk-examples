#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            ${board_root}/${board}/srtm_config.h
	    ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/cm7/rpmsg_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            ${board_root}/${board}/cm7/app_srtm.c
            ${board_root}/${board}/cm7/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/frdmimx952
             middleware/multicore/remoteproc
	     ${board_root}/frdmimx952/cm7
             ${board_root}/frdmimx952/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             ${board_root}/frdmimx952/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
	     ${board_root}/frdmimx952/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/cm7
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)

mcux_add_configuration(
        CC "-DSCMI_LMM_POWER_CHANGE_PROCESSED=1\
	    -DI2C_RETRY_TIMES=40000\
	    -DSRTM_IO_SERVICE_USED=1\
	    -DSRTM_I2C_SERVICE_USED=1"
)

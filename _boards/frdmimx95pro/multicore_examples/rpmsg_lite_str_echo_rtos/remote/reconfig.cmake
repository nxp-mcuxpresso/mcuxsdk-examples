#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
            ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
            ${board_root}/${board}/srtm_config.h
	    ${board_root}/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/remote/cm7/rpmsg_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            ${board_root}/${board}/cm7/app_srtm.c
            ${board_root}/${board}/cm7/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/frdmimx95pro
             middleware/multicore/remoteproc
	     ${board_root}/frdmimx95pro/cm7
             ${board_root}/frdmimx95pro/multicore_examples/rpmsg_lite_str_echo_rtos
             ${board_root}/frdmimx95pro/multicore_examples/rpmsg_lite_str_echo_rtos/remote
	     ${board_root}/frdmimx95pro/multicore_examples/rpmsg_lite_str_echo_rtos/remote/cm7
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)

mcux_add_configuration(
        CC "-DSCMI_LMM_POWER_CHANGE_PROCESSED=1"
)

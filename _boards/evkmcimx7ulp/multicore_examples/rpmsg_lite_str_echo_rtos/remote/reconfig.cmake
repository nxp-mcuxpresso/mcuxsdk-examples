mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/drivers/lpi2c/fsl_lpi2c_freertos.c
            ${board_root}/${board}/drivers/lpi2c/fsl_lpi2c_freertos.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            ${board_root}/${board}/srtm_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmcimx7ulp
             ${board_root}/${board}/drivers/lpi2c
             ${board_root}/evkmcimx7ulp/multicore_examples/rpmsg_lite_str_echo_rtos
             ${board_root}/evkmcimx7ulp/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)

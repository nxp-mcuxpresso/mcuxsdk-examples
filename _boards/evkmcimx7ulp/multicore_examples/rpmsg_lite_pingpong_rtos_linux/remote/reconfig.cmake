mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/FreeRTOSConfigBoard.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
            ${board_root}/${board}/srtm_config.h
            ${board_root}/${board}/rsc_table.c
            ${board_root}/${board}/rsc_table.h
            ${board_root}/${board}/app_srtm.c
            ${board_root}/${board}/app_srtm.h
            middleware/issdk/algorithms/pedometer/include/libinclude/KeynetikPedometer.h
            middleware/issdk/algorithms/pedometer/source/pedometer.c
            middleware/issdk/algorithms/pedometer/include/pedometer.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/issdk/algorithms/pedometer/include/libinclude
             middleware/issdk/algorithms/pedometer/source
             middleware/issdk/algorithms/pedometer/include
)

mcux_add_library(
        BASE_PATH ${SdkRootDirPath}
        LIBS middleware/issdk/algorithms/pedometer/lib/libpedometerm4.a
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmcimx7ulp
             ${board_root}/evkmcimx7ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             ${board_root}/evkmcimx7ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)


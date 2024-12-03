mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FreeRTOSConfigBoard.h
            examples/_boards/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/FreeRTOSConfig.h
            examples/_boards/${board}/multicore_examples/rpmsg_lite_str_echo_rtos/remote/rpmsg_config.h
            examples/_boards/${board}/srtm_config.h
            examples/_boards/${board}/rsc_table.c
            examples/_boards/${board}/rsc_table.h
            examples/_boards/${board}/app_srtm.c
            examples/_boards/${board}/app_srtm.h
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
    INCLUDES examples/_boards/evkmcimx7ulp
             examples/_boards/evkmcimx7ulp/multicore_examples/rpmsg_lite_str_echo_rtos
             examples/_boards/evkmcimx7ulp/multicore_examples/rpmsg_lite_str_echo_rtos/remote
)


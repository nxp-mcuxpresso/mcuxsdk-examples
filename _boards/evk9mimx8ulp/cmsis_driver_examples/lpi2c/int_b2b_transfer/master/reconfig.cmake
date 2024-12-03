mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/hardware_init.c
            examples/_boards/evkmimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/app.h
            examples/_boards/evkmimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master/RTE_Device.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/cmsis_driver_examples/lpi2c/int_b2b_transfer/master
             examples/_boards/evkmimx8ulp
)

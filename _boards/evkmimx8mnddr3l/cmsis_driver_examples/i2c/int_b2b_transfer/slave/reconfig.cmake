mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/i2c/int_b2b_transfer/slave/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/pin_mux.c
            examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/pin_mux.h
            examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/hardware_init.c
            examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/app.h
            examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave/RTE_Device.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/cmsis_driver_examples/i2c/int_b2b_transfer/slave
             examples/_boards/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/ecspi/polling_b2b_transfer/slave/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/ecspi/polling_b2b_transfer/slave/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/ecspi/polling_b2b_transfer/slave/./app.h
            examples/_boards/evkmimx8mn/driver_examples/ecspi/polling_b2b_transfer/slave/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/ecspi/polling_b2b_transfer/slave
             examples/_boards/evkmimx8mn
)

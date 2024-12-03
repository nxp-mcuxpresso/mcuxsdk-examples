
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/./hardware_init.c
            examples/_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/ecspi/ecspi_loopback
             examples/_boards/evkmimx8mn
)

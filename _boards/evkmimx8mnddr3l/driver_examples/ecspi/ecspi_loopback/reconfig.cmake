
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/./hardware_init.c
            ${board_root}/evkmimx8mn/driver_examples/ecspi/ecspi_loopback/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/ecspi/ecspi_loopback
             ${board_root}/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/flexio/spi/int_lpspi_transfer/slave/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/flexio/spi/int_lpspi_transfer/slave/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/flexio/spi/int_lpspi_transfer/slave
             ${board_root}/evkmimx8ulp
)

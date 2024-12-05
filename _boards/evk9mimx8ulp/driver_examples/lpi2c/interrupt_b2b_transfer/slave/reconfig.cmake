
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/lpi2c/interrupt_b2b_transfer/slave/./app.h
            ${board_root}/evkmimx8ulp/driver_examples/lpi2c/interrupt_b2b_transfer/slave/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/lpi2c/interrupt_b2b_transfer/slave
             ${board_root}/evkmimx8ulp
)

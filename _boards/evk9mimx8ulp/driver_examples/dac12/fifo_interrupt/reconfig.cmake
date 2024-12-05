
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/dac12/fifo_interrupt/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/dac12/fifo_interrupt/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/dac12/fifo_interrupt
             ${board_root}/evkmimx8ulp
)

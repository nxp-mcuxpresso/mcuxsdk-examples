
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/uart/interrupt/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/uart/interrupt/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/uart/interrupt/./hardware_init.c
            ${board_root}/evkmimx8mn/driver_examples/uart/interrupt/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/uart/interrupt
             ${board_root}/evkmimx8mn
)

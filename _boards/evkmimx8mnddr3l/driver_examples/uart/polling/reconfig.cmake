
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/uart/polling/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/uart/polling/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/uart/polling/./hardware_init.c
            ${board_root}/evkmimx8mn/driver_examples/uart/polling/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/uart/polling
             ${board_root}/evkmimx8mn
)

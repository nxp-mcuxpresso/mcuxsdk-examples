
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/uart/auto_baudrate_detect/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/uart/auto_baudrate_detect/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/uart/auto_baudrate_detect/./hardware_init.c
            examples/_boards/evkmimx8mn/driver_examples/uart/auto_baudrate_detect/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/uart/auto_baudrate_detect
             examples/_boards/evkmimx8mn
)

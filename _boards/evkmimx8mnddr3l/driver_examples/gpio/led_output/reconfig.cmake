
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/gpio/led_output/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/gpio/led_output/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/gpio/led_output/./hardware_init.c
            examples/_boards/evkmimx8mn/driver_examples/gpio/led_output/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/gpio/led_output
             examples/_boards/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/rgpio/input_interrupt/./hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/rgpio/input_interrupt/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/rgpio/input_interrupt
             examples/_boards/evkmimx8ulp
)

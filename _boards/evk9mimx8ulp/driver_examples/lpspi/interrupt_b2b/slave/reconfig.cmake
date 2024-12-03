
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/lpspi/interrupt_b2b/slave/./app.h
            examples/_boards/evkmimx8ulp/driver_examples/lpspi/interrupt_b2b/slave/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/lpspi/interrupt_b2b/slave
             examples/_boards/evkmimx8ulp
)

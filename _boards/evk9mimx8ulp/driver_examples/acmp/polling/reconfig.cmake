
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/acmp/polling/./hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/acmp/polling/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/acmp/polling
             examples/_boards/evkmimx8ulp
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/sdma/scatter_gather/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/sdma/scatter_gather/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/sdma/scatter_gather/./hardware_init.c
            examples/_boards/evkmimx8mn/driver_examples/sdma/scatter_gather/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/sdma/scatter_gather
             examples/_boards/evkmimx8mn
)

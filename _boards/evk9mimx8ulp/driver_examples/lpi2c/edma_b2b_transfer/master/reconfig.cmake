
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/lpi2c/edma_b2b_transfer/master/./app.h
            examples/_boards/evkmimx8ulp/driver_examples/lpi2c/edma_b2b_transfer/master/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/lpi2c/edma_b2b_transfer/master
             examples/_boards/evkmimx8ulp
)

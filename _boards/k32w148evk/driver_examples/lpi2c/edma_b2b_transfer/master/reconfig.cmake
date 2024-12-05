
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/driver_examples/lpi2c/edma_b2b_transfer/master/./hardware_init.c
            ${board_root}/kw45b41zevk/driver_examples/lpi2c/edma_b2b_transfer/master/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/driver_examples/lpi2c/edma_b2b_transfer/master/.
)

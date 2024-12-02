
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/driver_examples/edma/scatter_gather/./hardware_init.c
            examples/_boards/kw45b41zevk/driver_examples/edma/scatter_gather/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/driver_examples/edma/scatter_gather/.
)

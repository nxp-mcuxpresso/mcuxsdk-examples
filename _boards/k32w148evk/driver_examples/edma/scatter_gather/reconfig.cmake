
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/driver_examples/edma/scatter_gather/./hardware_init.c
            ${board_root}/kw45b41zevk/driver_examples/edma/scatter_gather/./app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/driver_examples/edma/scatter_gather/.
)

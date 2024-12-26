
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/edma/scatter_gather/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/edma/scatter_gather/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/edma/scatter_gather
             ${board_root}/evkmimx8ulp
)

mcux_add_armgcc_configuration(
    AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
)
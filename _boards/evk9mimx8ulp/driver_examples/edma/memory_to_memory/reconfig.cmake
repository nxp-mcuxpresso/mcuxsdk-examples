
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/edma/memory_to_memory/./hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/edma/memory_to_memory/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/edma/memory_to_memory
             examples/_boards/evkmimx8ulp
)

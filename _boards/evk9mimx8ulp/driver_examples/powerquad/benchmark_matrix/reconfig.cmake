
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/powerquad/benchmark_matrix/./hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/powerquad/benchmark_matrix/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/powerquad/benchmark_matrix
             examples/_boards/evkmimx8ulp
)

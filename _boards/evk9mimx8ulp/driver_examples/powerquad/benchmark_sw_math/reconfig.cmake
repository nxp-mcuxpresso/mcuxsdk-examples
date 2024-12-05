
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/powerquad/benchmark_sw_math/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/powerquad/benchmark_sw_math/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/powerquad/benchmark_sw_math
             ${board_root}/evkmimx8ulp
)

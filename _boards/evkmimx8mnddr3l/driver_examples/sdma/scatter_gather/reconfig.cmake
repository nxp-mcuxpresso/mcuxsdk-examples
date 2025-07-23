
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/sdma/scatter_gather/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/sdma/scatter_gather/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/sdma/scatter_gather/./hardware_init.c
            ${board_root}/evkmimx8mn/driver_examples/sdma/scatter_gather/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/sdma/scatter_gather
             ${board_root}/evkmimx8mn
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/sai/sdma_transfer/pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/sai/sdma_transfer/pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/sai/sdma_transfer/./app.h
            ${board_root}/evkmimx8mn/driver_examples/sai/sdma_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/sai/sdma_transfer
             ${board_root}/evkmimx8mn
)

mcux_remove_armgcc_configuration(
                AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

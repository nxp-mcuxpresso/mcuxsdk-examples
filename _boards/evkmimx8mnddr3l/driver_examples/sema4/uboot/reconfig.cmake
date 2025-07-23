
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/driver_examples/sema4/uboot/./pin_mux.c
            ${board_root}/evkmimx8mn/driver_examples/sema4/uboot/./pin_mux.h
            ${board_root}/evkmimx8mn/driver_examples/sema4/uboot/./hardware_init.c
            ${board_root}/evkmimx8mn/driver_examples/sema4/uboot/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/driver_examples/sema4/uboot/.
             ${board_root}/evkmimx8mn
)
include(${SdkRootDirPath}/examples/_boards/${board}/project_common.cmake OPTIONAL)

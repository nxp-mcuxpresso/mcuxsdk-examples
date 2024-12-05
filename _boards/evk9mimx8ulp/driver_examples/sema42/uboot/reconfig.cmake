
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/sema42/uboot/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/sema42/uboot/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/sema42/uboot/.
             ${board_root}/evkmimx8ulp
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/trdc/basic/hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/trdc/basic/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/trdc/basic
             ${board_root}/evkmimx8ulp
)

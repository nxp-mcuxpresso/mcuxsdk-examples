
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/driver_examples/wdog32/./hardware_init.c
            ${board_root}/evkmimx8ulp/driver_examples/wdog32/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/driver_examples/wdog32
             ${board_root}/evkmimx8ulp
)

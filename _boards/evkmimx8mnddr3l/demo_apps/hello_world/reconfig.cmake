
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8mn/demo_apps/hello_world/pin_mux.c
            ${board_root}/evkmimx8mn/demo_apps/hello_world/pin_mux.h
            ${board_root}/evkmimx8mn/demo_apps/hello_world/hardware_init.c
            ${board_root}/evkmimx8mn/demo_apps/hello_world/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8mn/demo_apps/hello_world
             ${board_root}/evkmimx8mn
)

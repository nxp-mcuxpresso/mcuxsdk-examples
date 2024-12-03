
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/demo_apps/hello_world/pin_mux.c
            examples/_boards/evkmimx8mn/demo_apps/hello_world/pin_mux.h
            examples/_boards/evkmimx8mn/demo_apps/hello_world/hardware_init.c
            examples/_boards/evkmimx8mn/demo_apps/hello_world/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/demo_apps/hello_world
             examples/_boards/evkmimx8mn
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/mbw/main.c
            examples/demo_apps/mbw/mbw.c
            examples/demo_apps/mbw/mbw_utils.c
            examples/demo_apps/mbw/mbw_port.c
            examples/demo_apps/mbw/mbw.h
            examples/_boards/${board}/demo_apps/mbw/mbw_def.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/mbw
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

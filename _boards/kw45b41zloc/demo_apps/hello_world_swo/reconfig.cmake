
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/demo_apps/hello_world_swo/hardware_init.c
            examples/_boards/kw45b41zevk/demo_apps/hello_world_swo/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/demo_apps/hello_world_swo
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/demo_apps/hello_world_swo/hardware_init.c
            ${board_root}/kw45b41zevk/demo_apps/hello_world_swo/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/demo_apps/hello_world_swo
)

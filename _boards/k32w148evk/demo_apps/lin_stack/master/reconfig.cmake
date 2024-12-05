
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/lin_stack/master/lin_stack_master.c
            ${board_root}/kw45b41zevk/demo_apps/lin_stack/master/lin_cfg.c
            ${board_root}/kw45b41zevk/demo_apps/lin_stack/master/lin_cfg.h
            ${board_root}/kw45b41zevk/demo_apps/lin_stack/master/hardware_init.c
            ${board_root}/kw45b41zevk/demo_apps/lin_stack/master/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/demo_apps/lin_stack/master
)

mcux_add_macro(
    CC "-DFSL_SDK_LIN_STACK_ENABLE=1"
)

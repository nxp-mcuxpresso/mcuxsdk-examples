
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/lin_stack/slave/lin_stack_slave.c
            examples/_boards/kw45b41zevk/demo_apps/lin_stack/slave/lin_cfg.c
            examples/_boards/kw45b41zevk/demo_apps/lin_stack/slave/lin_cfg.h
            examples/_boards/kw45b41zevk/demo_apps/lin_stack/slave/hardware_init.c
            examples/_boards/kw45b41zevk/demo_apps/lin_stack/slave/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/demo_apps/lin_stack/slave
)

mcux_add_macro(
    CC "-DFSL_SDK_LIN_STACK_ENABLE=1"
)

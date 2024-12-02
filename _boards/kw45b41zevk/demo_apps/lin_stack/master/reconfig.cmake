
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/lin_stack/master/lin_cfg.c
            examples/_boards/${board}/demo_apps/lin_stack/master/lin_cfg.h
)

mcux_add_macro(
    CC "-DFSL_SDK_LIN_STACK_ENABLE=1"
)

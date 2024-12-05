
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/lin_stack/slave/lin_cfg.c
            ${board_root}/${board}/demo_apps/lin_stack/slave/lin_cfg.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/lin_stack/slave/${core_id}
)

mcux_add_macro(
    CC "-DFSL_SDK_LIN_STACK_ENABLE=1"
)

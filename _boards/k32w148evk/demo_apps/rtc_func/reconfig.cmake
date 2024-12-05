
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/demo_apps/rtc_func/hardware_init.c
            ${board_root}/kw45b41zevk/demo_apps/rtc_func/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/demo_apps/rtc_func
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=200"
)

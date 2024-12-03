
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/demo_apps/rtc_func/hardware_init.c
            examples/_boards/kw45b41zevk/demo_apps/rtc_func/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/demo_apps/rtc_func
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=200"
)

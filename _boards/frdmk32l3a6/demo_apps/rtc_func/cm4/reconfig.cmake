
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/rtc_func
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=200"
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/rtc_func_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/rtc_func_peripheral/peripherals.h
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_PRINTF_MAX_LOG_LEN=200\
       -DFRDM_MCXC242\
       -DFREEDOM"
)

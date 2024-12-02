
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/sigfox/boards/${board}/sigfox_console/pin_mux.c
            middleware/sigfox/boards/${board}/sigfox_console/pin_mux.h
            middleware/sigfox/boards/${board}/sigfox_console/main.c
            middleware/sigfox/boards/${board}/sigfox_console/sf_cmd.c
            middleware/sigfox/boards/${board}/sigfox_console/sf_cmd.h
            middleware/sigfox/boards/${board}/board.c
            middleware/sigfox/boards/${board}/board.h
            middleware/sigfox/boards/${board}/clock_config.c
            middleware/sigfox/boards/${board}/clock_config.h
            examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/sigfox/boards/${board}/sigfox_console
             middleware/sigfox/boards/frdmk32l2b
)

mcux_add_macro(
    CC "-DFRDM_K32L2B\
       -DFREEDOM\
       -DPRINTF_FLOAT_ENABLE=0\
       -DSCANF_FLOAT_ENABLE=0\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=0"
)

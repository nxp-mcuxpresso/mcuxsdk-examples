
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_macro(
    CC "-DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING\
       -DFRDM_K32L2B\
       -DFREEDOM"
)

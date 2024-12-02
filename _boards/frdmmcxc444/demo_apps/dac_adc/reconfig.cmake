
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FlashKLxx256KROM_with_config_write_enabled.board
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DFRDM_MCXC444\
       -DFREEDOM"
)

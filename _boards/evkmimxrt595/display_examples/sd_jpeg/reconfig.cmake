mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/display_examples/sd_jpeg/libjpeg_user_config.h
)

mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
       -DSSD1963_DATA_WITDH=8\
       -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8\
       -DLIBJPEG_USER_CONFIG_FILE=\\\"libjpeg_user_config.h\\\"\
       -DDEMO_RM67162_USE_RG565\
       -DDEMO_SSD1963_USE_RG565"
)

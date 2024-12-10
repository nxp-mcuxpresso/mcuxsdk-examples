# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
        -DUSE_DBI=1\
        -DSSD1963_DATA_WITDH=8"
)
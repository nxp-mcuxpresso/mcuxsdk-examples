# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
       -DSSD1963_DATA_WITDH=8\
       -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8"
)

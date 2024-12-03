# Add additional configuration
mcux_add_macro(
    CC "-DBOARD_ENABLE_PSRAM_CACHE=0\
        -DSDK_I2C_BASED_COMPONENT_USED=1"
)
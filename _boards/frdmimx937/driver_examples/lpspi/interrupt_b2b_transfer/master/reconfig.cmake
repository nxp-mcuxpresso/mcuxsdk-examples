# Enable PCA6416A (I2C-based expander) used on FRDM i.MX937.
mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)


# Remove targets inherited from arch/soc/board level
mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCAL6408=1"
)

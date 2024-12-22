
mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_mdk_configuration(
    TARGETS debug
    CC "-Oz"
)

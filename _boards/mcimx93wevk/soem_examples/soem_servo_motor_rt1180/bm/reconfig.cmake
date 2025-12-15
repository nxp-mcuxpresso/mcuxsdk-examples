
# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DEC_MAXSLAVE=32\
       -DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCAL6524=1"
)
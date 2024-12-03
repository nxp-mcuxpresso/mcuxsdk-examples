mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_gpio_pulse/freertos/soem_gpio_pulse.c
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_gpio_pulse/freertos
)




# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)
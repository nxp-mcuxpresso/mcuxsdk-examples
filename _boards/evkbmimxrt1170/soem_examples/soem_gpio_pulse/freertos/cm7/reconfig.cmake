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
       -DPRINTF_ADVANCED_ENABLE=1\
       -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
       -DBOARD_NETWORK_USE_100M_ENET_PORT=1"
)

mcux_add_armgcc_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
)

mcux_add_iar_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
)
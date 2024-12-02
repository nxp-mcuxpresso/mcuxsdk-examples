mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x1000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_source(
    SOURCES nxp_iot_agent_flash_config.h
)
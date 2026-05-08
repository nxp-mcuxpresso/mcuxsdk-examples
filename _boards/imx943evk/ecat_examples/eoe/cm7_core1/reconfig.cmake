
# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
        -DNETC_PROMISCUOUS=1\
        -DFSL_ETH_ENABLE_CACHE_CONTROL\
        -DMAX_MBX_QUEUE_SIZE=100\
        -DEOE_RECEIVE_TASK_PRIORITY=3\
        -DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_armgcc_configuration(
    TARGETS debug release  
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-O0"
    CC "-DLWIP_TIMEVAL_PRIVATE=0"
)

mcux_add_iar_configuration(
    TARGETS debug release  
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
    CC "-DLWIP_TIMEVAL_PRIVATE=1"
)

mcux_add_mdk_configuration(
    TARGETS debug release  
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-O0"
)

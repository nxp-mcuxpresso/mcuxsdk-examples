mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/ecat_examples/eoe/eoe_freertos.c
            examples/ecat_examples/eoe/FreeRTOSConfig.h
            examples/_boards/${board}/FreeRTOSConfigBoard.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ecat_examples/eoe
    INCLUDES examples/ecat_examples/eoe/webpage
    INCLUDES examples/_boards/${board}/ecat_examples/eoe/${core_id}
)




# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
        -DNETC_PROMISCUOUS=1\
        -DFSL_ETH_ENABLE_CACHE_CONTROL\
        -DMAX_MBX_QUEUE_SIZE=100\
        -DEOE_RECEIVE_TASK_PRIORITY=3\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    TOOLCHAINS IAR
    CC "-DLWIP_TIMEVAL_PRIVATE=1"
)

mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "-Xlinker --defsym=__stack_size__=0x2048 -Xlinker --defsym=__heap_size__=0x30000"
    CC "-O0"
    CC "-DLWIP_TIMEVAL_PRIVATE=0"
)

mcux_add_iar_configuration(
    TARGETS flexspi_nor_debug flexspi_nor_release
    LD "--config_def=__stack_size__=0x2048 --config_def=__heap_size__=0x30000"
    CC "-DLWIP_TIMEVAL_PRIVATE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_servo_motor/bm/soem_servo_motor.c
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_servo_motor/bm
)




# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
        -DEC_MAXSLAVE=32\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_armgcc_configuration(
    TARGETS debug flexspi_nor_debug
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
)

mcux_add_armgcc_configuration(
    TARGETS release flexspi_nor_release
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-O0"
)

mcux_add_iar_configuration(
    TARGETS debug flexspi_nor_debug release flexspi_nor_release
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug flexspi_nor_debug
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
)

mcux_add_mdk_configuration(
    TARGETS release flexspi_nor_release
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-O0"
)

# mcux_remove_iar_configuration(
#     TARGETS release
#     CC "-Oh"
# )

# mcux_remove_iar_configuration(
#     TARGETS flexspi_nor_release
#     CC "-Om"
# )

# mcux_add_iar_configuration(
#     TARGETS release flexspi_nor_release
#     CC "-On"
# )
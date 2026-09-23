
# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)

# Uncomment to use the LPI2C based ESC EEPROM emulator instead of the on board
# EEPROM. ECAT_SDA/ECAT_SCL are then muxed on GPIO_IO14/GPIO_IO15 and must be
# wired to LPI2C4 (GPIO_IO18/GPIO_IO19), see example_board_readme.md.
# mcux_add_macro(
#     CC "-DEEPROM_I2C_EMULATOR"
# )

mcux_add_armgcc_configuration(
    TARGETS debug release  
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-O0"
)

mcux_add_iar_configuration(
    TARGETS debug release  
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug release  
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-O0"
)
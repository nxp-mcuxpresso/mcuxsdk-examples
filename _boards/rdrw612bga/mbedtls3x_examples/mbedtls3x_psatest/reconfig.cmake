#armgcc configurations
mcux_add_armgcc_configuration(
    CC "-Os"
)

#iar configurations
mcux_remove_iar_configuration(
    TARGETS debug flash_debug
    CX "-On"
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS debug flash_debug
    CC "-Ohz"
    CX "-Ohz"
)

#mdk configurations
mcux_add_mdk_configuration(
    CC "-Oz"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000\
        -Xlinker --defsym=__heap_size__=0x8000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000\
        --config_def=__heap_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)

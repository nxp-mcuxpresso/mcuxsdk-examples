mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000\
        --config_def=__heap_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x4000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000\
        -Xlinker --defsym=__heap_size__=0x8000"
)

# Define the __pkc__ linker symbol to enable PKC RAM offset in linker
mcux_add_iar_configuration(
    LD "--config_def __pkc__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__pkc__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__pkc__=1"
)

# ARMGCC debug target optimalization bumped to O1, so it fits into memory
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)

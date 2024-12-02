
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x400"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)

# MDK linker flags
mcux_add_mdk_configuration(
    LD "--legacyalign"
)
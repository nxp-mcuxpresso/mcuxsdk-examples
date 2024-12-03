
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x1000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)
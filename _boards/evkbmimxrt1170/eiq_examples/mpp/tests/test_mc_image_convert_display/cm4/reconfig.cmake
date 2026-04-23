# board+application specific cmake

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __heap_size__=0x400 \
            "
)

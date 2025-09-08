
mcux_add_macro(
    CC -DFREEDOM
)

mcux_remove_linker_symbol(
    SYMBOLS __stack_size__=0x2000
)

mcux_add_linker_symbol(
    SYMBOLS __stack_size__=0x4000
)
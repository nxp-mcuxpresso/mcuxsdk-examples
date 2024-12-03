# Override stack size
mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=2048"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)

# Override stack size
mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=2048"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)

mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
)

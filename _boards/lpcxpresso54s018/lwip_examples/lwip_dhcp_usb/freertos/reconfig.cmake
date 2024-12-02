mcux_add_macro(
    CC "-DUSB_STACK_USE_DEDICATED_RAM=1"
    AS "-DIMG_BAUDRATE=96000000"
)

# Override stack size
mcux_remove_linker_symbol(
    SYMBOLS "__stack_size__=2048"
)
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000"
)

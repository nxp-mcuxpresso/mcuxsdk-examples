
# Define macro
mcux_add_macro(
    CC  "USB_STACK_USE_DEDICATED_RAM=1"
)
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x1000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)

# Define macro
mcux_add_macro(
    CC  PHY_LAN8741=1
)
# Define macro
mcux_add_macro(
    CC  ENET_NO_PREAMBLE_PARAM=1
)
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x7000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)
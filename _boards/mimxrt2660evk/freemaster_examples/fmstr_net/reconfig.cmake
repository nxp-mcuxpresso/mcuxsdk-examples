
# Define macro
mcux_add_macro(
    CC  "PHY_YT8521=1 BOARD_USE_PCAL6524=1 SDK_I2C_BASED_COMPONENT_USED=1"
)

# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x7000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)


# Define macro
mcux_add_macro(
    CC  PHY_KSZ8081=1
)
# Define macro
mcux_add_macro(
    CC  FSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE
)
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x7000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)
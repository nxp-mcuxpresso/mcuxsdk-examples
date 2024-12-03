
# Define macro
mcux_add_macro(
    CC  PHY_RTL8211F=1
)
# Heap configuration
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=0x7000"
)
# Stack configuration
mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x800"
)
# Define macro
mcux_add_macro(
    CC  "FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1  FSL_ETH_ENABLE_CACHE_CONTROL=1"
)
# Add additional configuration
mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL\
       -DETH_LINK_POLLING_INTERVAL_MS=0"
)
mcux_add_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-O0"
)

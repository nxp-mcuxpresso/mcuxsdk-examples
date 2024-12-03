# Add additional configuration
mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL=1"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

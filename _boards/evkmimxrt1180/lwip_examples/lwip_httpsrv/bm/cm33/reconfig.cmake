# Add additional configuration
mcux_add_macro(
    CC "-DMEMP_NUM_IGMP_GROUP=4\
        -DFSL_ETH_ENABLE_CACHE_CONTROL"
)
mcux_add_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-Og"
)
mcux_add_iar_configuration(
    TARGETS debug
    CC "-Om"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug hyperram_debug
    CC "-O0"
)
mcux_remove_iar_configuration(
    TARGETS debug
    CC "-On"
)

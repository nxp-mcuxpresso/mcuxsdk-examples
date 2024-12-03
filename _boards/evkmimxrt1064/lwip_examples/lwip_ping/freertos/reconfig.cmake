# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DETH_LINK_POLLING_INTERVAL_MS=0"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS sdram_debug sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

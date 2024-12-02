# Add additional configuration
mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DETH_LINK_POLLING_INTERVAL_MS=0\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_iar_configuration(
    TARGETS sdram_release
    CC "-Oh"
)

# Remove additional configuration
mcux_remove_iar_configuration(
    TARGETS sdram_release
    CC "-Om"
)

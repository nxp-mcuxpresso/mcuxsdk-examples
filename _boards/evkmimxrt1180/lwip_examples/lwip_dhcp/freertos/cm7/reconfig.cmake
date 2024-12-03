# Add additional configuration
mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL\
        -DETH_LINK_POLLING_INTERVAL_MS=0"
)

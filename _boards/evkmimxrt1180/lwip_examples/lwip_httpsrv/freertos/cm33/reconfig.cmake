# Add additional configuration
mcux_add_macro(
    CC "-DNETC_PROMISCUOUS=1\
        -DFSL_ETH_ENABLE_CACHE_CONTROL"
)

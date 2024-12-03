# Add additional configuration
mcux_add_macro(
    CC "-DMEMP_NUM_IGMP_GROUP=4\
        -DFSL_ETH_ENABLE_CACHE_CONTROL"
)

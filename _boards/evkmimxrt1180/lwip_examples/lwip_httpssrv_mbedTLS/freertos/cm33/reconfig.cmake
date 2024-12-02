# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_KEY_EXCHANGE_RSA_ENABLED\
        -DFSL_ETH_ENABLE_CACHE_CONTROL"
)

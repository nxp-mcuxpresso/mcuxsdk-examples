# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        -DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256\
        -DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)

# Override heap size
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=40000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=50000"
)

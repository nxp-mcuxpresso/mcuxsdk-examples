# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DLWIP_ENET_FLEXIBLE_CONFIGURATION\
        -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        -DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256"
)
mcux_add_macro(
    TOOLCHAINS iar mdk armgcc
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

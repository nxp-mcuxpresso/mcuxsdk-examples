# Add additional configuration
mcux_add_macro(
    CC "-DLWIP_ENET_FLEXIBLE_CONFIGURATION\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

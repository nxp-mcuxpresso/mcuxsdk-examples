# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Override heap size
mcux_remove_linker_symbol(
    SYMBOLS "__heap_size__=40000"
)
mcux_add_linker_symbol(
    SYMBOLS "__heap_size__=50000"
)

# Add additional configuration
mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL\
        -DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256\
        -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

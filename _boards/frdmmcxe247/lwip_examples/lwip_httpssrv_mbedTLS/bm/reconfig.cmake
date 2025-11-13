# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256"
)

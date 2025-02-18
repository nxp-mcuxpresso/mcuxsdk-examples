# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        -DMBEDTLS_SSL_CIPHERSUITES=MBEDTLS_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)
mcux_add_mdk_configuration(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    LD "--predefine=\"-DXIP_BOOT_HEADER_ENABLE=0\""
)

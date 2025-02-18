# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1\
       -g1"
    CX "-g1"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0\
       -g"
    CX "-g"
)

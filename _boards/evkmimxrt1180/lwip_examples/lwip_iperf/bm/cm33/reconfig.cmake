# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

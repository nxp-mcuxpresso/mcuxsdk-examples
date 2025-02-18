# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
        -DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

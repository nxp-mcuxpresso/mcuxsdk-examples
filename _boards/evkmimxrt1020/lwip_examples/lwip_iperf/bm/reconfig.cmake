# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_macro(
    TOOLCHAINS mdk
    TARGETS sdram_release
    CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

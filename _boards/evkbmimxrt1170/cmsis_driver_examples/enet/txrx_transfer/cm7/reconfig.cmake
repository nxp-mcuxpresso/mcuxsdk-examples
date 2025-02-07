# Copyright 2024-2025 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DPHY_STABILITY_DELAY_US=500000U\
       -DUSE_SDRAM\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)

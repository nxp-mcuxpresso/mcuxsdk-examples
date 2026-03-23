# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)
mcux_add_macro(
    TOOLCHAINS iar armgcc
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DLWIPERF_QUICKACCESS_TX_BUFFER=0"
)


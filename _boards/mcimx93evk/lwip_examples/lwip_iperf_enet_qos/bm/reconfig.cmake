# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Add additional configuration
mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_PCAL6524=1"
)

mcux_add_macro(
    TOOLCHAINS iar
    CC "-DLWIPERF_QUICKACCESS_TX_BUFFER=0"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

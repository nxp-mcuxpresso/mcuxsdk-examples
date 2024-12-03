#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# Remove no_se and/or m33 from IAR FLags
mcux_remove_iar_configuration(
        AS "--cpu=cortex-m33.no_se"
        CC "--cpu=cortex-m33.no_se"
        CX "--cpu=cortex-m33.no_se"
        LD "--cpu=cortex-m33.no_se"
)
#
## And then this will add Trust Zone Enable clicky button checked in IAR GUI Project
mcux_add_iar_configuration(
        AS "--cpu=cortex-m33.no_dsp"
        CC "--cpu=cortex-m33.no_dsp"
        CX "--cpu=cortex-m33.no_dsp"
        LD "--cpu=cortex-m33.no_dsp"
)

#as MCXN9XX platform has flash available
mcux_remove_macro(
    TOOLCHAINS armgcc iar mdk
    TARGETS debug release
    CC "-DPLATFORM_NO_FLASH"
)

#
# Copyright 2025 NXP
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
    AS "--cpu=cortex-m33"
    CC "--cpu=cortex-m33"
    CX "--cpu=cortex-m33"
    LD "--cpu=cortex-m33"
)
#as MCXN2XX platform has flash available
#mcux_remove_macro(
#    TOOLCHAINS armgcc iar mdk mcux
#    TARGETS debug release
#    CC "-DPLATFORM_NO_FLASH"
#)

#as MCXN2XX platform has flash available
mcux_add_macro(
    TOOLCHAINS armgcc iar mdk mcux
    TARGETS debug release
    CC "-DFSL_SDK_FORCE_CLK_DRIVER_NS_ACCESS=1\
	-DDISABLE_WDOG=1"
)

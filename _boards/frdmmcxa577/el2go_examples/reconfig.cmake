#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
# Remove no_se and/or m33 from IAR FLags
mcux_remove_iar_configuration(
    AS "--cpu=cortex-m33.no_se"
    CC "--cpu=cortex-m33.no_se"
    CX "--cpu=cortex-m33.no_se"
    LD "--cpu=cortex-m33.no_se"
)

# And then this will add Trust Zone Enable clicky button checked in IAR GUI Project
mcux_add_iar_configuration(
    AS "--cpu=cortex-m33"
    CC "--cpu=cortex-m33"
    CX "--cpu=cortex-m33"
    LD "--cpu=cortex-m33"
)

#as MCXL255 platform has flash available
mcux_remove_macro(
    TOOLCHAINS armgcc iar mdk mcux
    TARGETS debug release
    CC "-DPLATFORM_NO_FLASH"
)

# Remove default linker files
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_remove_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

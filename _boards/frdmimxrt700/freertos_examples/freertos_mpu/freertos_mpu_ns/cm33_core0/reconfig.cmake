#
# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
mcux_add_include(
    INCLUDES ./
)

mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_mdk_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_mdk_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_remove_iar_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.ld
)

mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.scf
)

mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_ns.icf
)

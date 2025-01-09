# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_remove_iar_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_add_iar_linker_script(
        TARGETS flash_debug flash_release
        LINKER ./linkers/RW612_flash.icf
)

mcux_remove_armgcc_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    LINKER ./linkers/RW612_flash.ld
)

mcux_remove_mdk_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    LINKER ./linkers/RW612_flash.scf
)

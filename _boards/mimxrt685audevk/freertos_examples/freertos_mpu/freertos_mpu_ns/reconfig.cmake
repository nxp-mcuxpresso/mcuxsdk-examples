#
# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
    INCLUDES ./
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/iar/MIMXRT685Sxxxx_cm33_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/arm/MIMXRT685Sxxxx_cm33_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/RT/RT600/MIMXRT685S/gcc/MIMXRT685Sxxxx_cm33_ram.ld
)

mcux_add_iar_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_ram_ns.icf
)
mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_ram_ns.ld
)
mcux_add_mdk_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_ram_ns.scf
)
mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_flash_ns.icf
)
mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_flash_ns.ld
)
mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT685Sxxxx_cm33_flash_ns.scf
)

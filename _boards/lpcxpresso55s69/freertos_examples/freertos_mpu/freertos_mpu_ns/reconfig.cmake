#
# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES hardware_init.c
            app.h
)

mcux_add_include(
    INCLUDES ./
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
)

mcux_add_iar_linker_script(
    TARGETS debug release
    LINKER linkscripts/LPC55S69_cm33_core0_flash_ns.icf
)
mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER linkscripts/LPC55S69_cm33_core0_flash_ns.ld
)
mcux_add_mdk_linker_script(
    TARGETS debug release
    LINKER linkscripts/LPC55S69_cm33_core0_flash_ns.scf
)

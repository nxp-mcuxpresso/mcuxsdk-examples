#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/
)

mcux_add_include(
    INCLUDES ./
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_ram.ld
)


mcux_add_iar_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_ram_ns.icf
)
mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_ram_ns.ld
)
mcux_add_mdk_linker_script(
    TARGETS debug release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_ram_ns.scf
)
mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_flash_ns.icf
)
mcux_add_armgcc_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_flash_ns.ld
)
mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    LINKER linkscripts/MIMXRT595Sxxxx_cm33_flash_ns.scf
)

mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../freertos_mpu_s/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
    GENERATED TRUE
)

#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES hardware_init.c
            app.h
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/freertos_config_board/${core_id}/FreeRTOSConfigBoard.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/freertos_config_board/${core_id}/
)
mcux_add_include(
    INCLUDES ./
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
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

mcux_add_source(
    SOURCES exdata.ldt
            main_text_section.ldt
            main_data_section.ldt
    TOOLCHAINS mcux
)


mcux_add_library(
    BASE_PATH ${APPLICATION_BINARY_DIR}
    LIBS "../freertos_mpu_s/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
    GENERATED TRUE
)
#
# Copyright 2024-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
    CC "-DDEMO_CODE_START_NS=65536 "
    )

# Remove no_se from IAR FLags
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

mcux_add_source(
    SOURCES nsc_functions.c
            nsc_functions.h
            tzm_config.c
            tzm_config.h
)

mcux_add_source(
    SOURCES hardware_init.c
            app.h
            pin_mux.c
            pin_mux.h
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
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash_s.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash_s.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash_s.scf
)

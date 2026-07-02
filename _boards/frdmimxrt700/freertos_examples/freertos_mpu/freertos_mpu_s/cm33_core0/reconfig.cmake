#
# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES ../nsc_functions.c
            ../nsc_functions.h
            tzm_config.c
            tzm_config.h
            secureport_override_armgcc.h
)

mcux_add_include(
    INCLUDES ./
)

mcux_add_macro(
    CC "-DDEMO_CODE_START_NS=672137216 -DBOARD_TZM_SG_IN_SRAM"
)

# For ARMGCC builds, force-include a project-local header that overrides
# secureportNON_SECURE_CALLABLE to place NSC-callable bodies into .sg_ramfunc
# when BOARD_TZM_SG_IN_SRAM is enabled.
# Apply for both CLI ARMGCC builds (CONFIG_TOOLCHAIN=armgcc) and MCUXpresso IDE
# project generation/builds (CONFIG_TOOLCHAIN=mcux).
mcux_add_armgcc_mcux_configuration(
    CC "-include secureport_override_armgcc.h"
    CX "-include secureport_override_armgcc.h"
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
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.ld
)

mcux_add_mdk_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.scf
)

mcux_add_iar_linker_script(
    TARGETS flash_debug flash_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/trustzone_examples/linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.icf
)

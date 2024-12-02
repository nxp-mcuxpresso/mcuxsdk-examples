#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_${core_id}_flash_s.ld
)

mcux_add_iar_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_${core_id}_flash_s.icf
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS flash_debug flash_release
    CC "-DDEMO_CODE_START_NS=68157440"
)

mcux_add_macro(
    TOOLCHAINS iar
    TARGETS flash_debug
    CC "-DIAR_FP_VLSTM_ASSIGNED_ISSUE=1"
)

mcux_add_source(
    SOURCES hardware_init.c
            app.h
            tzm_config.c
            tzm_config.h
)

mcux_add_include(
    INCLUDES .
)

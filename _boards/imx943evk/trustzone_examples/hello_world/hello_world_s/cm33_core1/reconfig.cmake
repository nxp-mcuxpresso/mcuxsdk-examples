#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.ld
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.icf
)
mcux_add_source(
    SOURCES tzm_config.c
            tzm_config.h
            hardware_init.c
            app.h

)
mcux_add_include(
    INCLUDES .
)

mcux_add_macro(
    TOOLCHAINS armgcc iar
    TARGETS debug release
    CC "-DDEMO_CODE_START_NS=268304384"
)

mcux_add_macro(
    TOOLCHAINS iar
    TARGETS debug
    CC "-DIAR_FP_VLSTM_ASSIGNED_ISSUE=1"
)

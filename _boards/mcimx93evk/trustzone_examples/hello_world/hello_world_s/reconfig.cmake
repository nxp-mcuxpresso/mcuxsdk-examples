#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_${core_id}_ram_s.ld
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_${core_id}_ram_s.icf
)

mcux_add_source(
    SOURCES tzm_config.c
            tzm_config.h
)
mcux_add_include(
    INCLUDES .
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS debug release
    AS "-DDEMO_CODE_START_NS=268369920"
    CC "-DDEMO_CODE_START_NS=268369920"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS debug
    CC "-DDEMO_CODE_START_NS=268369920\
       -DIAR_FP_VLSTM_ALIGNED_ISSUE=1"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS release
    CC "-DDEMO_CODE_START_NS=268369920"
)

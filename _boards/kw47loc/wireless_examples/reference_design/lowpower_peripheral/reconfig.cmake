# Copyright 2024-2025 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
    INCLUDES
    ./
    ../../
)

mcux_add_source(
    PREINCLUDE TRUE
    SOURCES app_preinclude.h
)

mcux_add_macro(ECC_RAM_STCM8_INIT=1)

mcux_add_linker_symbol(SYMBOLS "gLowpowerPowerDownEnable_d=1")

mcux_add_macro(RPMSG_ADAPTER_NON_BLOCKING_MODE=1)

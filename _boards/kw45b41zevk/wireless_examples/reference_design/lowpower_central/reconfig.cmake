# Copyright 2024 NXP
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


mcux_add_linker_symbol(SYMBOLS "gLowpowerPowerDownEnable_d=1")

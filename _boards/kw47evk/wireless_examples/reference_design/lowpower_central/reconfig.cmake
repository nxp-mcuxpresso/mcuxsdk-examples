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

mcux_add_macro(ECC_RAM_STCM8_INIT=1)

# Power down mode is not supported yet on low power reference design applications
# do not define gLowpowerPowerDownEnable_d

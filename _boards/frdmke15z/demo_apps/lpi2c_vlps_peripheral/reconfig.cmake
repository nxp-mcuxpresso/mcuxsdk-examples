# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/lpi2c_vlps_peripheral/peripherals.c
            ${board_root}/${board}/demo_apps/lpi2c_vlps_peripheral/peripherals.h
            ${board_root}/${board}/demo_apps/lpi2c_vlps_peripheral/clock_config.c
            ${board_root}/${board}/demo_apps/lpi2c_vlps_peripheral/clock_config.h
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DFRDM_KE15Z\
       -DFREEDOM"
)

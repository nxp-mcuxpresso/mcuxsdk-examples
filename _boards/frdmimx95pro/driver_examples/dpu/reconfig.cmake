# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DBOARD_USE_PCAL6524=1"
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx95pro/driver_examples/dpu/linker/MIMX9596_cm7_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX95/MIMX9596/iar/MIMX9596xxxxN_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx95pro/driver_examples/dpu/linker/MIMX9596_cm7_ram.icf
)

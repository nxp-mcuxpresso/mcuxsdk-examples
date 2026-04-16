# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_PCA6416A=1"
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX937/MIMX9375/gcc/MIMX9375_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx937/driver_examples/dpu/linker/MIMX9375_cm7_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX937/MIMX9375/iar/MIMX9375_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx937/driver_examples/dpu/linker/MIMX9375_cm7_ram.icf
)

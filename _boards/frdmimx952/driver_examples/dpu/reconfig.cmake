# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
    INCLUDES .
)

mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DBOARD_USE_PI4IO6408=1"
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/gcc/MIMX9529_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx952/driver_examples/dpu/linker/MIMX9529_cm7_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX9529/iar/MIMX9529_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/frdmimx952/driver_examples/dpu/linker/MIMX9529_cm7_ram.icf
)

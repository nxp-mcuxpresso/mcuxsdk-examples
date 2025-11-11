# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# setup armgcc linker
mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER examples/_boards/${board}/driver_examples/dpu/g2d_basic_test/cm7/linker_g2d.ld
)

# setup iar linker
mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX95/MIMX9596/iar/MIMX9596xxxxN_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER examples/_boards/${board}/driver_examples/dpu/g2d_basic_test/cm7/linker_g2d_iar.icf
)

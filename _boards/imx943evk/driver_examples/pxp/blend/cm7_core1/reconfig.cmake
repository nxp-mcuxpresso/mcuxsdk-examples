# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause



mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX943/MIMX94398/gcc/MIMX94398_cm7_core1_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/driver_examples/pxp/linker/MIMX94398_cm7_core1_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX943/MIMX94398/iar/MIMX94398_cm7_core1_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx943evk/driver_examples/pxp/linker/MIMX94398_cm7_core1_ram.icf
)

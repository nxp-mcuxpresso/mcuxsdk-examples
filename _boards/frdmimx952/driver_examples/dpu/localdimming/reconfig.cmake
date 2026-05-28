# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
    SOURCES ${board_root}/${board}/display_support.c
    SOURCES ${board_root}/${board}/driver_examples/dpu/localdimming/ld_support.c
    SOURCES ${board_root}/${board}/driver_examples/dpu/localdimming/ld_support.h
    SOURCES ${board_root}/${board}/driver_examples/dpu/localdimming/incbin.S
    SOURCES ${board_root}/${board}/driver_examples/dpu/localdimming/imx9xm0p_ld_fw.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dpu/localdimming/cm7
    INCLUDES ${board_root}/${board}/driver_examples/dpu/localdimming
)

mcux_add_macro(
	CC "-DLD_IMAGE_COPY_TO_RAM=1"
)

include(${SdkRootDirPath}/${board_root}/frdmimx952/driver_examples/dpu/reconfig.cmake)

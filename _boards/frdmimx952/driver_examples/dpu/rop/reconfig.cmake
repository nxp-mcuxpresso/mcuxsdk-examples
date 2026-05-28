# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
    SOURCES ${board_root}/${board}/display_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dpu/rop/cm7
)

include(${SdkRootDirPath}/${board_root}/frdmimx952/driver_examples/dpu/reconfig.cmake)

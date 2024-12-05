# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/dpu/reconfig.cmake)

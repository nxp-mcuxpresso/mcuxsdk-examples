# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/display_support.h
            ${board_root}/${board}/display_support.c
            ${board_root}/${board}/isi_board.c
            ${board_root}/${board}/isi_example.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/isi
             ${board_root}/${board}
)

# Copyright 2024 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/isi/dpu_board.c
            examples/_boards/${board}/isi_board.c
            examples/_boards/${board}/isi_example.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/isi
             examples/_boards/${board}
)

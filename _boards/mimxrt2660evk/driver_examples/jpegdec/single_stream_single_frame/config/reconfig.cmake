# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/jpegdec/pin_mux.c
            ${board_root}/${board}/driver_examples/jpegdec/pin_mux.h
)

mcux_add_macro(
    CC "-DDEMO_BUFFER_FIXED_ADDRESS=1"
)

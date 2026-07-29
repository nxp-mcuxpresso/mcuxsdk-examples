# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/isi_board.c
            ${board_root}/${board}/isi_example.h
            ${board_root}/${board}/driver_examples/isi/mipi_csi2/app_display.c
            ${board_root}/${board}/driver_examples/isi/mipi_csi2/app_display.h
            ${board_root}/${board}/display_support.c
            ${board_root}/${board}/display_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/isi/mipi_csi2
             ${board_root}/${board}/driver_examples/isi
             ${board_root}/${board}
             examples/drivers/mipi_csi
)

mcux_add_macro(CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1")

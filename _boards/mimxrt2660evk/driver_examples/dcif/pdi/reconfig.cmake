# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_PCAL6524=1"
)

mcux_add_source(
    SOURCES isi_config.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/isi_board.c
            ${board_root}/${board}/isi_example.h
            ${board_root}/${board}/driver_examples/dcif/pdi/dcif_support.c
            ${board_root}/${board}/driver_examples/dcif/pdi/dcif_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/dcif
)
